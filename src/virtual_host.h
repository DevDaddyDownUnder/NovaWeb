#ifndef NOVAWEB_VIRTUAL_HOST_H
#define NOVAWEB_VIRTUAL_HOST_H

#define MAX_HOSTS 10
#define MAX_HOST_SIZE 1024
#define MAX_DOCUMENT_ROOT_LENGTH 256

typedef struct {
    char host[MAX_HOST_SIZE];
    char document_root[MAX_DOCUMENT_ROOT_LENGTH];
} host_config;

void add_host_config(char *host, char *document_root);
void get_document_root(char host[MAX_HOST_SIZE], char *buffer, size_t buffer_size);

#endif //NOVAWEB_VIRTUAL_HOST_H
