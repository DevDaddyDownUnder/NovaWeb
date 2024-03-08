#ifndef NOVAWEB_REQUEST_H
#define NOVAWEB_REQUEST_H

#define MAX_METHOD_LENGTH 10
#define MAX_URI_LENGTH 1024
#define MAX_VERSION_LENGTH 20
#define MAX_HEADER_COUNT 20
#define MAX_HEADER_NAME_LENGTH 50
#define MAX_HEADER_VALUE_LENGTH 100
#define MAX_BODY_LENGTH 4096

typedef struct {
    char method[MAX_METHOD_LENGTH];
    char uri[MAX_URI_LENGTH];
    char version[MAX_VERSION_LENGTH];
    char headers[MAX_HEADER_COUNT][MAX_HEADER_NAME_LENGTH][MAX_HEADER_VALUE_LENGTH];
    int header_count;
    int content_length;
    char body[MAX_BODY_LENGTH];
} http_request;

void parse_request(char *buffer, http_request *request);
const char *get_header(http_request request,  char *header_name);
void print_request(http_request request);

#endif //NOVAWEB_REQUEST_H
