#ifndef NOVAWEB_FILE_H
#define NOVAWEB_FILE_H

#include <stdbool.h>
#include <stdio.h>

#define FILE_BUFFER_SIZE 4096

bool send_file(int client_socket, char *file_path, bool keep_alive);
bool send_file_contents(int client_socket, FILE *file_ptr);

#endif //NOVAWEB_FILE_H
