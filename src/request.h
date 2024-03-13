#ifndef NOVAWEB_REQUEST_H
#define NOVAWEB_REQUEST_H

#include "header.h"

typedef struct {
    char method[MAX_METHOD_LENGTH];
    char uri[MAX_URI_LENGTH];
    char version[MAX_VERSION_LENGTH];
    http_header headers[MAX_HEADER_COUNT];
    int header_count;
    long content_length;
    char body[MAX_BODY_LENGTH];
} http_request;

void parse_request(char *buffer, http_request *request);
void add_request_header(http_request *request, char *name, char *value);
char* get_header_value(http_request *request, char *header_name);
void print_request(http_request request);

#endif //NOVAWEB_REQUEST_H
