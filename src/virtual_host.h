#ifndef NOVAWEB_VIRTUAL_HOST_H
#define NOVAWEB_VIRTUAL_HOST_H

#define MAX_HOSTS 10
#define MAX_HOST_LENGTH 1024
#define MAX_DOCUMENT_ROOT_LENGTH 256

typedef struct
{
    char host[MAX_HOST_LENGTH];
    char document_root[MAX_DOCUMENT_ROOT_LENGTH];
} host_config;

/**
 * Add a new host config.
 *
 * @param host
 * @param document_root
 */
void add_host_config(char *host, char *document_root);

/**
 * Return the document root for the specified host.
 *
 * @param host
 * @return The document root for the host.
 */
char *get_document_root(char host[MAX_HOST_LENGTH]);

#endif //NOVAWEB_VIRTUAL_HOST_H
