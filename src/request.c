#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

// Parse the request from the buffer received from the client
void parse_request(char *buffer, http_request *request)
{
    // Parse request line
    sscanf(buffer, "%s %s %s", request->method, request->uri, request->version);

    // Default content length
    request->content_length = -1;

    // Parse headers
    char *line;
    int line_count = 0;

    // Tokenize the request by lines
    line = strtok(buffer, "\r\n");
    while (line != NULL)
    {
        // Check if the line contains a colon ':' to identify header lines
        char *colon = strchr(line, ':');
        if (colon != NULL)
        {
            // Extract the header name and value
            *colon = '\0';  // Null-terminate the header name
            char *header_name = line;
            char *header_value = colon + 1;

            // Copy the header name and value into the struct
            snprintf(request->headers[line_count][0], MAX_HEADER_NAME_LENGTH, "%s", header_name);
            snprintf(request->headers[line_count][1], MAX_HEADER_VALUE_LENGTH, "%s", header_value);

            // Check if the header is "Content-Length" to extract the content length
            if (strcmp(header_name, "Content-Length") == 0)
            {
                // Convert the header value to an integer
                char *endptr;
                long length = strtol(header_value, &endptr, 10);
                if (errno == ERANGE || *endptr != '\0' || length < 0)
                {
                    fprintf(stderr, "Error converting Content-Length to integer.\n");
                }
                else
                {
                    request->content_length = (int) length;
                }
            }

            // Increment the header count
            line_count++;
            request->header_count++;
        }

        // Move to the next line
        line = strtok(NULL, "\r\n");
    }
}

// Function to find a header by name
const char *get_header(http_request request, char *header_name)
{
    for (int i = 0; i < request.header_count; i++)
    {
        if (strcmp(request.headers[i][0], header_name) == 0)
        {
            return request.headers[i][1];
        }
    }

    return NULL;
}

// Helper function to debug requests
void print_request(http_request request)
{
    // Print the request line
    printf("Request line: %s %s %s %d\n", request.method, request.uri, request.version, request.content_length);

    // Print the headers
    for (int i = 0; i < MAX_HEADER_COUNT; i++)
    {
        if (strlen(request.headers[i][0]) < 1)
        {
            break;
        }

        printf("Header: %s, Value: %s\n", request.headers[i][0], request.headers[i][1]);
    }

    printf("\n");
}