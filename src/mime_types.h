#ifndef HTTPSERVER_MIME_TYPES_H
#define HTTPSERVER_MIME_TYPES_H

#include <stdio.h>
#include <string.h>

#define MAX_MIME_TYPES 80
#define MAX_EXTENSION_SIZE 10
#define MAX_MIME_TYPE_SIZE 100

typedef struct {
    char extension[MAX_EXTENSION_SIZE];
    char mime_type[MAX_MIME_TYPE_SIZE];
} mime_mapping;

extern mime_mapping mime_types[MAX_MIME_TYPES];

void get_mime_type(char *filename, char *buffer, size_t buffer_size);

#endif //HTTPSERVER_MIME_TYPES_H
