#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "http_server.h"

int main(int argc, char *argv[]);

void print_welcome(void);

// Entry point.
int main(int argc, char *argv[])
{
    int opt;
    int port = DEFAULT_PORT;

    // Parse options
    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
            case 'p':
            {
                char *endptr;
                long int port_long = strtol(optarg, &endptr, 10);

                // Check the port is in bounds
                if (endptr == optarg || *endptr != '\0' || port_long < 0 || port_long > 65535)
                {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return 1;
                }

                port = (int) port_long;
                break;
            }
            case '?':
            {
                // Invalid option or missing argument
                if (optopt == 'p')
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else
                {
                    fprintf(stderr, "Unknown option -%c.\n", optopt);
                }
                return 1;
            }
            default:
                fprintf(stderr, "Usage: %s [-p <port>]\n", argv[0]);
                return 1;
        }
    }

    // Welcome message
    print_welcome();

    // Start the server entering an infinite loop
    start_http_server(AF_INET, INADDR_ANY, port, SOCKET_BACKLOG);

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
