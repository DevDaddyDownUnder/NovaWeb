#ifndef NOVAWEB_HTTP_SERVER_H
#define NOVAWEB_HTTP_SERVER_H

#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define SOCKET_BACKLOG 100

void start_http_server(unsigned char domain, unsigned int interface, uint16_t port, int backlog);
void * handle_client(void *client_socket);
void handle_termination_signal(int signum);

#endif //NOVAWEB_HTTP_SERVER_H
