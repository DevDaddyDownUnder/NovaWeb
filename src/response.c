#include <stdio.h>
#include <string.h>
#include "response.h"

// Helper function to add headers to the response object
void add_header(http_response *response, char *name, char *value)
{
    // Check if there's space for another header
    if (response->header_count >= MAX_HEADER_COUNT)
    {
        perror("Header limit exceeded");
        return;
    }

    if (strlen(name) >= MAX_HEADER_NAME_LENGTH)
    {
        fprintf(stderr, "Header name exceeds maximum length (%d)\n", MAX_HEADER_NAME_LENGTH);
        return;
    }

    if (strlen(value) >= MAX_HEADER_VALUE_LENGTH)
    {
        fprintf(stderr, "Header value exceeds maximum length (%d)\n", MAX_HEADER_VALUE_LENGTH);
        return;
    }

    // Copy the header name and value into the response struct
    strncpy(response->headers[response->header_count].name, name, MAX_HEADER_NAME_LENGTH - 1);
    strncpy(response->headers[response->header_count].value, value, MAX_HEADER_VALUE_LENGTH - 1);

    // Ensure null-terminated
    response->headers[response->header_count].name[MAX_HEADER_NAME_LENGTH - 1] = '\0';
    response->headers[response->header_count].value[MAX_HEADER_VALUE_LENGTH - 1] = '\0';

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
        strcat(output, response->headers[i].name);
        strcat(output, ": ");
        // Header value
        strcat(output, response->headers[i].value);
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
               response.headers[i].name,
               response.headers[i].value);
    }

    // Print body
    printf("\n%s\n\n", response.body);
}
