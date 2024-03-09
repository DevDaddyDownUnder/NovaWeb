#ifndef NOVAWEB_RESPONSE_H
#define NOVAWEB_RESPONSE_H

#include "request.h"

#define HTTP_VERSION "1.1"
#define MAX_RESPONSE_LENGTH (MAX_BODY_LENGTH + MAX_HEADER_COUNT * (MAX_HEADER_NAME_LENGTH + MAX_HEADER_VALUE_LENGTH) + 100)

typedef struct {
    char name[MAX_HEADER_NAME_LENGTH];
    char value[MAX_HEADER_VALUE_LENGTH];
} http_header;

typedef struct {
    int status_code;
    char status_message[MAX_HEADER_VALUE_LENGTH];
    http_header headers[MAX_HEADER_COUNT];
    char body[MAX_BODY_LENGTH];
    int header_count;
} http_response;

void add_header(http_response *response, char *name, char *value);
void build_http_response(http_response *response, char *output);
void print_response(http_response request);

#endif //NOVAWEB_RESPONSE_H
