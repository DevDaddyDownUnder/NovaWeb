#ifndef NOVAWEB_FILE_H
#define NOVAWEB_FILE_H

#include <stdbool.h>
#include <stdio.h>

#define FILE_BUFFER_SIZE 4096

/**
 * Send the file to the client.
 *
 * @param client_socket
 * @param file_path Absolute path to the file that should be sent.
 * @param keep_alive The keep alive flag is used to add the Connection and Keep-Alive headers.
 * @return bool - true if succeeded, otherwise false.
 */
bool send_file(int client_socket, char *file_path, bool keep_alive);

/**
 * Read the files contents and send it to the client.
 *
 * @param client_socket
 * @param file_ptr
 * @return bool - true if succeeded, otherwise false.
 */
bool send_file_contents(int client_socket, FILE *file_ptr);

#endif //NOVAWEB_FILE_H
