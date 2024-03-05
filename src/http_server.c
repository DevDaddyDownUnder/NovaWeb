#include <dirent.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "http_server.h"

#define BUFFER_SIZE 1024

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
        printf("Error creating socket.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = domain;
    server_addr.sin_addr.s_addr = interface;
    server_addr.sin_port = htons(port);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error binding socket.\n");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, backlog) < 0)
    {
        printf("Error listening for connections.\n");
        exit(EXIT_FAILURE);
    }

    // User feedback
    printf("NovaWeb HTTP Server listening on: %d\n", port);

    // Main server loop
    while (1)
    {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            printf("Error accepting connection.\n");
            continue;
        }

        // Create a new thread to handle the client connection
        if (pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket) != 0)
        {
            printf("Error creating thread.\n");
            close(client_socket);
            continue;
        }

        // Detach the thread to allow it to run independently
        if (pthread_detach(thread_id) != 0)
        {
            printf("Error detaching thread.\n");
            close(client_socket);
            continue;
        }
    }

    // Close server socket
    close(server_socket);
}

// Handle the processing of a request sent from a client and return a response.
void *handle_client(void *arg)
{
    int client_socket = *((int *) arg);
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0)
    {
        printf("Error receiving data from client.\n");
        close(client_socket);
        pthread_exit(NULL);
    } else if (bytes_received == 0)
    {
        // Client closed the connection
        printf("Client closed the connection.\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Send HTTP response headers
    char response_headers[BUFFER_SIZE];
    snprintf(response_headers, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World!</h1></body></html>");

    int headers_length = strlen(response_headers);
    int bytes_sent = send(client_socket, response_headers, headers_length, 0);
    if (bytes_sent < headers_length)
    {
        printf("Error sending response headers.\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Close file and client socket
    close(client_socket);
    pthread_exit(NULL);
}