#include <stdio.h>
#include <netinet/in.h>
#include "http_server.h"

int main(void);

void print_welcome(void);

// Entry point.
int main(void)
{
    // Welcome message
    print_welcome();

    // Start the server entering an infinite loop
    start_http_server(AF_INET, INADDR_ANY, DEFAULT_PORT, SOCKET_BACKLOG);

    return 0;
}

// Welcome message.
void print_welcome(void)
{
    printf("  _   _              __          __  _     \n");
    printf(" | \\ | |             \\ \\        / / | |    \n");
    printf(" |  \\| | _____   ____ \\ \\  /\\  / /__| |__  \n");
    printf(" | . ` |/ _ \\ \\ / / _` \\ \\/  \\/ / _ \\ '_ \\ \n");
    printf(" | |\\  | (_) \\ V / (_| |\\  /\\  /  __/ |_) |\n");
    printf(" |_| \\_|\\___/ \\_/ \\__,_| \\/  \\/ \\___|_.__/ \n");
    printf("                              HTTP Server\n\n");
}
