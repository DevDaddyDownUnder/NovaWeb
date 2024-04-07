#ifndef HTTPSERVER_MIME_TYPES_H
#define HTTPSERVER_MIME_TYPES_H

#include <stdio.h>
#include <string.h>

#define MAX_MIME_TYPES 80
#define MAX_EXTENSION_LENGTH 10
#define MAX_MIME_TYPE_LENGTH 100

typedef struct
{
    char extension[MAX_EXTENSION_LENGTH];
    char mime_type[MAX_MIME_TYPE_LENGTH];
} mime_mapping;

extern mime_mapping mime_types[MAX_MIME_TYPES];

/**
 * Get the mime type based on the file extension, fallback is text/plain.
 *
 * @param filename
 * @return The mime type based on the file extension, fallback is text/plain.
 */
char *get_mime_type(char *filename);

#endif //HTTPSERVER_MIME_TYPES_H
