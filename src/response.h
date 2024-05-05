#ifndef NOVAWEB_RESPONSE_H
#define NOVAWEB_RESPONSE_H

#include "header.h"

typedef struct
{
    int status_code;
    char status_message[MAX_HEADER_VALUE_LENGTH];
    http_header headers[MAX_HEADER_COUNT];
    int header_count;
    long content_length;
    char body[MAX_BODY_LENGTH];
} http_response;

/**
 * Helper function to add headers to the response object.
 *
 * @param response
 * @param name
 * @param value
 */
void add_response_header(http_response *response, char *name, char *value);

/**
 * Build the response string based on the response object.
 *
 * @param response
 * @param output
 */
void build_http_response(http_response *response, char *output);

/**
 * Helper function to debug response.
 *
 * @param request
 */
void print_response(http_response request);

/**
 * Helper function to send not found.
 *
 * @param client_socket
 */
void send_not_found(int client_socket);

#endif //NOVAWEB_RESPONSE_H
