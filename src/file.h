#ifndef NOVAWEB_FILE_H
#define NOVAWEB_FILE_H

#include <stdio.h>

#define FILE_BUFFER_SIZE 4096

void send_file(int client_socket, char *file_path);
void send_file_contents(int client_socket, FILE *file_ptr);
void send_chunked_file_contents(int client_socket, FILE *file_ptr);
int send_chunk(int client_socket, char *buffer, size_t size);

#endif //NOVAWEB_FILE_H
