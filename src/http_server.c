#include <dirent.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/mman.h>
#include "http_server.h"
#include "config.h"
#include "directory.h"
#include "file.h"
#include "path.h"
#include "request.h"
#include "virtual_host.h"
#include "url.h"

// Prevent the compiler from optimising this variable away
volatile sig_atomic_t *stop_server = 0;

// Main logic to set up the socket and listen for new connections.
void start_http_server(unsigned char domain, unsigned int interface, uint16_t port, int backlog, long core_count)
{
    int server_socket;
    struct sockaddr_in server_addr;

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Enable the SO_REUSEADDR option
    int enable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("Error setting socket to reuse address");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = domain;
    server_addr.sin_addr.s_addr = interface;
    server_addr.sin_port = htons(port);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, backlog) < 0)
    {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    // Set server socket to non-blocking
    if (fcntl(server_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("Error setting server socket to non-blocking");
        exit(EXIT_FAILURE);
    }

    // User feedback
    printf("NovaWeb HTTP Server listening on: %d\n\n", port);

    // Set up signal handlers
    signal(SIGINT, handle_termination_signal);
    signal(SIGTERM, handle_termination_signal);

    // Create shared memory segment for stop_server flag
    stop_server = mmap(NULL, sizeof(*stop_server), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (stop_server == MAP_FAILED)
    {
        perror("Error creating shared memory");
        exit(EXIT_FAILURE);
    }

    // Create new process for each core
    if (multi_process_flag)
    {
        pid_t workers[core_count];
        for (int i = 0; i < core_count; i++)
        {
            pid_t pid = fork();

            if (pid == -1)
            {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // Child process

                // Blocking call
                accept_connections(server_socket);
                exit(EXIT_SUCCESS);
            }
            else
            {
                // Parent process
                workers[i] = pid;
            }
        }

        // Wait for child processes to terminate
        for (int i = 0; i < core_count; i++)
        {
            waitpid(workers[i], NULL, 0);
        }
    }
    else
    {
        // Blocking call
        accept_connections(server_socket);
    }

    // Gracefully shut down the server
    printf("Shutting down server...\n");

    // Close server socket
    close(server_socket);
}

// Accept connection loop.
void accept_connections(int server_socket)
{
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Set up timeout for select()
    struct timeval timeout;
    timeout.tv_sec = 1; // TODO work out a good value for this
    timeout.tv_usec = 0;
    int ready;

    // Main server loop
    while (!*stop_server)
    {
        // Set up file descriptor set for select() with server socket
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);

        // Wait for incoming connection with timeout
        ready = select(server_socket + 1, &readfds, NULL, NULL, &timeout);
        if (ready < 0)
        {
            break;
        }
        else if (ready == 0)
        {
            // Timeout reached, check for termination signals
            continue;
        }

        // Accept incoming connection
        if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len)) < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // No incoming connection, continue
                continue;
            }
            perror("Error accepting connection");
            continue;
        }

        // Dynamically allocate memory for client_socket
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        // Create a new thread to handle the client connection
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *) client_socket_ptr) != 0)
        {
            perror("Error creating thread");
            free(client_socket_ptr);
            close(client_socket);
            continue;
        }

        // Detach the thread to allow it to run independently
        if (pthread_detach(thread_id) != 0)
        {
            perror("Error detaching thread");
            free(client_socket_ptr);
            close(client_socket);
            continue;
        }
    }
}

// Handle the processing of a request sent from a client and return a response.
void *handle_client(void *arg)
{
    int client_socket = *((int *) arg);

    // Set client socket to non-blocking
    int flags = fcntl(client_socket, F_GETFL, 0);
    if (flags == -1)
    {
        perror("Error getting socket flags");
        close(client_socket);
        free(arg);
        pthread_exit(NULL);
    }
    if (fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("Error setting socket to non-blocking");
        close(client_socket);
        free(arg);
        pthread_exit(NULL);
    }

    char buffer[BUFFER_SIZE * 2];
    ssize_t bytes_received;
    bool keep_alive = true;
    time_t last_request_time;
    int request_count = 0;

    // Set timeout for receive
    struct pollfd fds[1];
    fds[0].fd = client_socket;
    fds[0].events = POLLIN;

    while (keep_alive)
    {
        int activity = poll(fds, 1, KEEP_ALIVE_TIMEOUT_SECONDS * 1000);
        if (activity == -1)
        {
            perror("Error in poll");
            break;
        }
        else if (activity == 0)
        {
            // Timeout occurred
            keep_alive = false;
            break;
        }
        else
        {
            // Receive data from client
            bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // No data available at the moment
                    continue;
                }
                else if (errno != EBADF)
                {
                    // Bad file descriptor is occurring with JMeter spam sometimes
                    perror("Error receiving data from client");
                }

                close(client_socket);
                free(arg);
                pthread_exit(NULL);
            }
            else if (bytes_received == 0)
            {
                // Client closed the connection
                close(client_socket);
                free(arg);
                pthread_exit(NULL);
            }

            // Update last request time
            last_request_time = time(NULL);
            request_count++;

            // Parse request
            http_request request;
            memset(&request, 0, sizeof(http_request));
            parse_request(buffer, &request);
            if (verbose_flag)
            {
                print_request(request);
            }

            // Extract requested file
            int built_path = build_file_path(request.uri);
            if (built_path == 0)
            {
                // TODO send a 400?
                perror("Error building file path");
                close(client_socket);
                free(arg);
                pthread_exit(NULL);
            }

            // Build absolute file path based on document root
            char *host = get_header_value(&request, "Host");

            // Get document root
            char file_path[BUFFER_SIZE];
            char *document_root = get_document_root(host);
            if (strcmp(request.uri, "/") == 0)
            {
                strncpy(file_path, document_root, BUFFER_SIZE - 1);
                file_path[BUFFER_SIZE - 1] = '\0';
            }
            else
            {
                char temp_path[BUFFER_SIZE];
                int temp_path_length = snprintf(temp_path, BUFFER_SIZE, "%s/%s", document_root, request.uri);
                if (temp_path_length >= BUFFER_SIZE || temp_path_length < 0)
                {
                    perror("Error combined length of document root and request uri exceed the buffer");
                    close(client_socket);
                    free(arg);
                    pthread_exit(NULL);
                }

                // Copy the concatenated path into file_path
                strncpy(file_path, temp_path, BUFFER_SIZE);
                file_path[BUFFER_SIZE - 1] = '\0';
            }

            // Set the default file if it exists
            if (directory_listing_flag && strchr(file_path, '.') == NULL)
            {
                // Calculate the length of the default file path
                size_t default_file_length = strlen(file_path) + strlen(DEFAULT_FILE) + 1;

                // Check if the combined length exceeds the buffer size
                if (default_file_length > BUFFER_SIZE)
                {
                    perror("Error combined length of file path and default file exceed the buffer");
                    close(client_socket);
                    free(arg);
                    pthread_exit(NULL);
                }

                // Concatenate file_path and DEFAULT_FILE into default_file
                char default_file[BUFFER_SIZE];
                // -2 to leave space for '/', '\0'
                strncpy(default_file, file_path, BUFFER_SIZE - strlen(DEFAULT_FILE) - 2);
                // Add '/'
                default_file[strlen(default_file)] = '/';
                // +1 for the null terminator
                strncpy(default_file + strlen(default_file), DEFAULT_FILE, strlen(DEFAULT_FILE) + 1);

                // Check if the default file exists
                struct stat st;
                memset(&st, 0, sizeof(struct stat));
                if (stat(default_file, &st) == 0)
                {
                    // Copy the concatenated path back to file_path
                    strncpy(file_path, default_file, BUFFER_SIZE);
                    file_path[BUFFER_SIZE - 1] = '\0';
                }
            }

            // URL decode the file_path, names with special characters and URL encoded
            url_decode(file_path, file_path, BUFFER_SIZE);

            if (verbose_flag)
            {
                printf("Requested file: %s\n", request.uri);
                printf("File path: %s\n", file_path);
                printf("Request count on client-socket: %i - %i\n\n", client_socket, request_count);
            }

            // Check if the connection should be kept alive
            char *connection_header = get_header_value(&request, "Connection");
            if (strcmp(connection_header, "") == 0 || strcasecmp(connection_header, "close") == 0)
            {
                keep_alive = false;
            }

            // Check for keep-alive timeout or request count exceeded
            if (difftime(time(NULL), last_request_time) > KEEP_ALIVE_TIMEOUT_SECONDS ||
                request_count > KEEP_ALIVE_MAX_REQUESTS)
            {
                keep_alive = false;
            }

            // Check if the requested file is a directory
            struct stat st;
            memset(&st, 0, sizeof(struct stat));
            stat(file_path, &st);

            // Send directory list or file
            if (directory_listing_flag && S_ISDIR(st.st_mode))
            {
                // Send directory listing
                send_directory_listing(client_socket, file_path, document_root, keep_alive);
            }
            else if (!directory_listing_flag && S_ISDIR(st.st_mode))
            {
                // Not allowed directory access at this point
                send_not_found(client_socket);
            }
            else
            {
                // Send the requested file
                send_file(client_socket, file_path, keep_alive);
            }

            // Reset buffer for next request
            if (keep_alive)
            {
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }

    // Cleanup
    close(client_socket);
    free(arg);
    pthread_exit(NULL);
}

// Signal handler for termination signals
void handle_termination_signal(int signum)
{
    // Reset signal handler
    signal(signum, SIG_DFL);

    *stop_server = 1;
}
