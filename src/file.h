#ifndef NOVAWEB_FILE_H
#define NOVAWEB_FILE_H

#include <stdio.h>

#define FILE_BUFFER_SIZE 4096

void send_file_contents(int client_socket, FILE *file_ptr);
void send_file(int client_socket, char *file_path);

#endif //NOVAWEB_FILE_H
