#include <stdio.h>
#include <string.h>
#include "response.h"

// Helper function to add headers to the response object
void add_header(http_response *response, char *name, char *value)
{
    // Check if there's space for another header
    if (response->header_count >= MAX_HEADER_COUNT)
    {
        perror("Header limit exceeded.");
        return;
    }

    // Copy the header name and value into the response struct
    strncpy(response->headers[response->header_count][0], name, MAX_HEADER_NAME_LENGTH);
    strncpy(response->headers[response->header_count][1], value, MAX_HEADER_VALUE_LENGTH);

    // Ensure null-terminated
    response->headers[response->header_count][0][MAX_HEADER_NAME_LENGTH - 1] = '\0';
    response->headers[response->header_count][1][MAX_HEADER_VALUE_LENGTH - 1] = '\0';

    response->header_count++;
}

// Build the response string based on the response object
void build_http_response(http_response *response, char *output)
{
    // Build status line
    sprintf(output, "HTTP/%s %d %s\r\n",
            HTTP_VERSION,
            response->status_code,
            response->status_message);

    // Add headers
    for (int i = 0; i < response->header_count; i++)
    {
        // Header name
        strcat(output, response->headers[i][0]);
        strcat(output, ": ");
        // Header value
        strcat(output, response->headers[i][1]);
        strcat(output, "\r\n");
    }

    // Add blank line before body
    strcat(output, "\r\n");

    // Add body
    strcat(output, response->body);
}

// Helper function to debug response
void print_response(http_response response)
{
    // Print status line
    printf("HTTP/%s %d %s\n",
            HTTP_VERSION,
            response.status_code,
            response.status_message);

    // Print headers
    for (int i = 0; i < response.header_count; i++)
    {
        printf("%s: %s\n",
               response.headers[i][0],
               response.headers[i][1]);
    }

    // Print body
    printf("\n%s\n\n", response.body);
}
