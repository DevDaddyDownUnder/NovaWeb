#include <stdio.h>
#include <netinet/in.h>
#include "http_server.h"

void print_welcome();

// Entry point.
int main()
{
    // Welcome message
    print_welcome();

    // Start the server entering an infinite loop
    start_http_server(AF_INET, INADDR_ANY, DEFAULT_PORT, SOCKET_BACKLOG);

    return 0;
}

// Welcome message.
void print_welcome()
{
    printf("  _   _              __          __  _     \n");
    printf(" | \\ | |             \\ \\        / / | |    \n");
    printf(" |  \\| | _____   ____ \\ \\  /\\  / /__| |__  \n");
    printf(" | . ` |/ _ \\ \\ / / _` \\ \\/  \\/ / _ \\ '_ \\ \n");
    printf(" | |\\  | (_) \\ V / (_| |\\  /\\  /  __/ |_) |\n");
    printf(" |_| \\_|\\___/ \\_/ \\__,_| \\/  \\/ \\___|_.__/ \n");
    printf("                              HTTP Server\n\n");
}
