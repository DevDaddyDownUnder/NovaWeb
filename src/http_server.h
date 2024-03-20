#ifndef NOVAWEB_HTTP_SERVER_H
#define NOVAWEB_HTTP_SERVER_H

#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define SOCKET_BACKLOG 128
#define BUFFER_SIZE 1024

void start_http_server(unsigned char domain, unsigned int interface, uint16_t port, int backlog, long core_count);
void accept_connections(int server_socket);
void * handle_client(void *client_socket);
void handle_termination_signal(int signum);

#endif //NOVAWEB_HTTP_SERVER_H
