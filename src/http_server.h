#ifndef NOVAWEB_HTTP_SERVER_H
#define NOVAWEB_HTTP_SERVER_H

#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define SOCKET_BACKLOG 128
#define BUFFER_SIZE 1024

/**
 * Main logic to set up the socket and listen for new connections.
 *
 * @param domain
 * @param interface
 * @param port
 * @param backlog
 * @param core_count
 */
void start_http_server(unsigned char domain, unsigned int interface, uint16_t port, int backlog, long core_count);

/**
 * Accept connection loop.
 *
 * @param server_socket
 */
void accept_connections(int server_socket);

/**
 * Handle the processing of a request sent from a client and return a response.
 *
 * @param client_socket
 */
void *handle_client(void *client_socket);

/**
 * Signal handler for termination signals.
 *
 * @param signum
 */
void handle_termination_signal(int signum);

#endif //NOVAWEB_HTTP_SERVER_H
