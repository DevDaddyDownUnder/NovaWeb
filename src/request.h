#ifndef NOVAWEB_REQUEST_H
#define NOVAWEB_REQUEST_H

#include "header.h"

typedef struct
{
    char method[MAX_METHOD_LENGTH];
    char uri[MAX_URI_LENGTH];
    char version[MAX_VERSION_LENGTH];
    http_header headers[MAX_HEADER_COUNT];
    int header_count;
    long content_length;
    char body[MAX_BODY_LENGTH];
} http_request;

/**
 * Parse the request from the buffer received from the client.
 *
 * @param buffer
 * @param request
 */
void parse_request(char *buffer, http_request *request);

/**
 * Helper function to add headers to the request object.
 *
 * @param request
 * @param name
 * @param value
 */
void add_request_header(http_request *request, char *name, char *value);

/**
 * Function to find a header by name.
 *
 * @param request
 * @param header_name
 * @return The value of the header based on the name.
 */
char *get_header_value(http_request *request, char *header_name);

/**
 * Helper function to debug requests.
 *
 * @param request
 */
void print_request(http_request request);

#endif //NOVAWEB_REQUEST_H
