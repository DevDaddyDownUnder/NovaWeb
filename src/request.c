#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "string.h"

// Parse the request from the buffer received from the client
void parse_request(char *buffer, http_request *request)
{
    // Parse request line
    sscanf(buffer, "%s %s %s", request->method, request->uri, request->version);

    // Default content length
    request->content_length = -1;

    // Tokenize the request by lines
    char *token;
    while ((token = strsep(&buffer, "\r\n")) != NULL)
    {
        // Check if the line contains a colon ':' to identify header lines
        char *colon = strchr(token, ':');
        if (colon != NULL)
        {
            // Extract the header name and value
            char *header_name = token;
            *colon = '\0';
            char *header_value = trim(colon + 1);

            // Check if the header is "Content-Length" to extract the content length
            if (strcmp(header_name, "Content-Length") == 0)
            {
                // Convert the header value to an integer
                char *endptr;
                long length = strtol(header_value, &endptr, 10);
                if (errno == ERANGE || *endptr != '\0' || length < 0)
                {
                    perror("Error converting Content-Length to integer");
                }
                else
                {
                    request->content_length = length;
                }
            }

            // Add header
            add_request_header(request, header_name, header_value);
        }
    }
}

// Helper function to add headers to the request object.
void add_request_header(http_request *request, char *name, char *value)
{
    // Check if there's space for another header
    if (request->header_count >= MAX_HEADER_COUNT)
    {
        perror("Header limit exceeded");
        return;
    }

    if (strlen(name) >= MAX_HEADER_NAME_LENGTH)
    {
        fprintf(stderr, "Request header name (%s) exceeds maximum length (%d)\n", name, MAX_HEADER_NAME_LENGTH);
        return;
    }

    if (strlen(value) >= MAX_HEADER_VALUE_LENGTH)
    {
        fprintf(stderr, "Request header value (%s) exceeds maximum length (%d)\n", value, MAX_HEADER_VALUE_LENGTH);
        return;
    }

    // Copy the header name and value into the response struct
    strncpy(request->headers[request->header_count].name, name, MAX_HEADER_NAME_LENGTH - 1);
    strncpy(request->headers[request->header_count].value, value, MAX_HEADER_VALUE_LENGTH - 1);

    // Ensure null-terminated
    request->headers[request->header_count].name[MAX_HEADER_NAME_LENGTH - 1] = '\0';
    request->headers[request->header_count].value[MAX_HEADER_VALUE_LENGTH - 1] = '\0';

    request->header_count++;
}

// Function to find a header by name
char* get_header_value(http_request *request, char *header_name)
{
    for (int i = 0; i < request->header_count; i++)
    {
        if (strcmp(request->headers[i].name, header_name) == 0)
        {
            return request->headers[i].value;
        }
    }

    // Header not found
    return "";
}

// Helper function to debug requests
void print_request(http_request request)
{
    // Print the request line
    printf("Request line: %s %s %s %ld\n", request.method, request.uri, request.version, request.content_length);

    // Print the headers
    for (int i = 0; i < MAX_HEADER_COUNT; i++)
    {
        if (strlen(request.headers[i].name) < 1)
        {
            break;
        }

        printf("Header: %s, Value: %s\n", request.headers[i].name, request.headers[i].value);
    }

    printf("\n");
}
