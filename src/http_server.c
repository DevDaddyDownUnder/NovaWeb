#include <dirent.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "http_server.h"
#include "file.h"
#include "path.h"
#include "request.h"
#include "virtual_host.h"
#include "config.h"

#define BUFFER_SIZE 1024

// Prevent the compiler from optimising this variable away
volatile sig_atomic_t stop_server = 0;

// Main logic to set up the socket and listen for new connections.
void start_http_server(int domain, u_long interface, int port, int backlog)
{
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
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

    // User feedback
    printf("NovaWeb HTTP Server listening on: %d\n\n", port);

    // Set up signal handler for termination signals
    signal(SIGINT, handle_termination_signal);
    signal(SIGTERM, handle_termination_signal);

    // Set up timeout for select()
    struct timeval timeout;
    timeout.tv_sec = 1; // TODO work out a good value for this
    timeout.tv_usec = 0;
    int ready;

    // Main server loop
    while (!stop_server)
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
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("Error accepting connection");
            continue;
        }

        // Create a new thread to handle the client connection
        if (pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket) != 0)
        {
            perror("Error creating thread");
            close(client_socket);
            continue;
        }

        // Detach the thread to allow it to run independently
        if (pthread_detach(thread_id) != 0)
        {
            perror("Error detaching thread");
            close(client_socket);
            continue;
        }
    }

    // Gracefully shut down the server
    printf("Shutting down server...\n");

    // Close server socket
    close(server_socket);
}

// Handle the processing of a request sent from a client and return a response.
void *handle_client(void *arg)
{
    int client_socket = *((int *) arg);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0)
    {
        perror("Error receiving data from client");
        close(client_socket);
        pthread_exit(NULL);
    }
    else if (bytes_received == 0)
    {
        // Client closed the connection
        perror("Client closed the connection");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Build request
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
        pthread_exit(NULL);
    }

    // Build absolute file path based on document root
    char file_path[BUFFER_SIZE];
    char host[BUFFER_SIZE];
    get_header_value(&request, "Host", host, sizeof(host));

    char document_root[BUFFER_SIZE];
    get_document_root(host, document_root, sizeof(document_root));
    snprintf(file_path, sizeof(file_path), "%s/%s", document_root, request.uri);

    if (verbose_flag)
    {
        printf("Requested file: %s\n", request.uri);
        printf("File path: %s\n\n", file_path);
    }

    // Send the requested file
    send_file(client_socket, file_path);

    // Close file and client socket
    close(client_socket);
    pthread_exit(NULL);
}

// Signal handler for termination signals
void handle_termination_signal(int signum)
{
    // Reset signal handler
    signal(signum, SIG_DFL);

    stop_server = 1;
}
