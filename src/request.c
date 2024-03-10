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
            // Should be + 1, TODO create a trim function
            char *header_value = colon + 2;

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
                    request->content_length = length;
                }
            }

            // Add header
            add_request_header(request, header_name, header_value);
        }

        // Move to the next line
        line = strtok(NULL, "\r\n");
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
void get_header_value(http_request *request, char *header_name, char *buffer, size_t buffer_size)
{
    for (int i = 0; i < request->header_count; i++)
    {
        if (strcmp(request->headers[i].name, header_name) == 0)
        {
            // Copy the header value into the buffer
            strncpy(buffer, request->headers[i].value, buffer_size);
            return;
        }
    }

    // Header not found, write an empty string to the buffer
    buffer[0] = '\0';
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
