#ifndef NOVAWEB_FILE_H
#define NOVAWEB_FILE_H

#include <stdbool.h>
#include <stdio.h>

#define FILE_BUFFER_SIZE 4096

bool send_file(int client_socket, char *file_path);
bool send_file_contents(int client_socket, FILE *file_ptr);
bool send_chunked_file_contents(int client_socket, FILE *file_ptr);
bool send_chunk(int client_socket, char *buffer, size_t size);

#endif //NOVAWEB_FILE_H
