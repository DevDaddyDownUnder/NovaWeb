#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "http_server.h"
#include "virtual_host.h"
#include "config.h"

int main(int argc, char *argv[]);

void print_welcome(void);

// Entry point.
int main(int argc, char *argv[])
{
    int opt;
    uint16_t port = DEFAULT_PORT;
    char *host = NULL;
    char *document_root = NULL;
    long core_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (core_count < 0)
    {
        core_count = 1;
    }

    const struct option long_options[] = {
            // Verbose option sets a flag
            {"verbose", no_argument, &verbose_flag, 1},
            {"dl", no_argument, &directory_listing_flag, 1},
            {"mp", no_argument, &multi_process_flag, 1},

            // These options don't set a flag
            {"port", required_argument, NULL, 'p'},
            {"host", required_argument, NULL, 'h'},
            {"document", required_argument, NULL, 'd'},
            {NULL, 0, NULL, 0}
    };

    int option_index = 0;

    // Parse options
    while ((opt = getopt_long(argc, argv, "p:h:d:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 0:
                if (long_options[option_index].flag != NULL)
                {
                    break;
                }
                break;
            case 'p':
            {
                char *endptr;
                long port_long = strtol(optarg, &endptr, 10);

                // Check the port is in bounds
                if (endptr == optarg || *endptr != '\0' || port_long < 0 || port_long > 65535)
                {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return 1;
                }

                port = (uint16_t) port_long;
                break;
            }
            case 'h':
                host = optarg;
                break;
            case 'd':
                document_root = optarg;
                break;
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
                fprintf(stderr, "Usage: %s [-p <port>] [-h <host>] [-d <document root>] [--verbose]\n", argv[0]);
                return 1;
        }
    }

    // Add virtual host configuration
    if (document_root != NULL)
    {
        char host_buffer[MAX_HOST_LENGTH];

        // Default the host if not provided
        if (host == NULL)
        {
            snprintf(host_buffer, sizeof(host_buffer), "%s:%d", "127.0.0.1", port);
        }
        else
        {
            snprintf(host_buffer, sizeof(host_buffer), "%s:%d", host, port);
        }

        host = host_buffer;
        add_host_config(host, document_root);
    }

    // Welcome message
    print_welcome();

    // Start the server entering an infinite loop
    start_http_server(AF_INET, INADDR_ANY, port, SOCKET_BACKLOG, core_count);

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
