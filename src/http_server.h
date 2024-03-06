#ifndef NOVAWEB_HTTP_SERVER_H
#define NOVAWEB_HTTP_SERVER_H

#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define SOCKET_BACKLOG 20

void start_http_server(int domain, u_long interface, int port, int backlog);
void * handle_client(void *client_socket);
void handle_termination_signal(int signum);

#endif //NOVAWEB_HTTP_SERVER_H
