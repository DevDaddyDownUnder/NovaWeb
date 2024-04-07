#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "response.h"
#include "http_status.h"
#include "config.h"

// Helper function to add headers to the response object.
void add_response_header(http_response *response, char *name, char *value)
{
    // Check if there's space for another header
    if (response->header_count >= MAX_HEADER_COUNT)
    {
        perror("Header limit exceeded");
        return;
    }

    if (strlen(name) >= MAX_HEADER_NAME_LENGTH)
    {
        fprintf(stderr, "Response header name (%s) exceeds maximum length (%d)\n", name, MAX_HEADER_NAME_LENGTH);
        return;
    }

    if (strlen(value) >= MAX_HEADER_VALUE_LENGTH)
    {
        fprintf(stderr, "Response header value (%s) exceeds maximum length (%d)\n", value, MAX_HEADER_VALUE_LENGTH);
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
    // Ensure a status code is set
    if (response->status_code == 0)
    {
        response->status_code = OK;
    }

    // Set the status message based on the status code
    if (strlen(response->status_message) == 0)
    {
        strcpy(response->status_message, get_status_message(response->status_code));
    }

    // TODO only need to create this value once per second or possibly once per handle client
    // TODO add a flag to toggle this header?
    // Add date header
    time_t current_time;
    struct tm tm_info;
    char date_buffer[30];
    time(&current_time);
    gmtime_r(&current_time, &tm_info);
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %H:%M:%S %Z", &tm_info);
    add_response_header(response, "Date", date_buffer);

    // TODO only need to create this value once
    // Add server header
    if (server_signature_flag)
    {
        char server_signature[12];
        sprintf(server_signature, "%s/%s",
                NOVAWEB_LABEL,
                NOVAWEB_VERSION);
        server_signature[11] = '\0';
        add_response_header(response, "Server", server_signature);
    }

    // TODO cache status lines as these will be reused a lot
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

// Helper function to send not found.
void send_not_found(int client_socket)
{
    http_response response = {0};
    response.status_code = NOT_FOUND;
    add_response_header(&response, "Content-Type", "text/html");
    add_response_header(&response, "Content-Length", "108");

    strncpy(response.body, "<!DOCTYPE html>"
                           "<html>"
                           "<head><title>404 - Not Found</title></head>"
                           "<body><h1>404 - Not Found</h1></body>"
                           "</html>", MAX_BODY_LENGTH - 1);
    response.body[MAX_BODY_LENGTH - 1] = '\0';

    // Build response as string
    char output[MAX_RESPONSE_LENGTH];
    build_http_response(&response, output);
    if (verbose_flag)
    {
        print_response(response);
    }

    // Send the response
    size_t response_length = strlen(output);
    ssize_t bytes_sent = send(client_socket, output, response_length, 0);
    if (bytes_sent < (ssize_t) response_length)
    {
        perror("Error sending response headers");
        close(client_socket);
        pthread_exit(NULL);
    }
}
