#ifndef NOVAWEB_DIRECTORY_H
#define NOVAWEB_DIRECTORY_H

#include "file.h"
#include "response.h"

/**
 * Send a list of files and directories in the specified path to the client.
 *
 * @param client_socket
 * @param directory_path Absolute path to the directory that should be sent.
 * @param document_root Document root is used to check if parent traversal is allowed.
 * @param keep_alive The keep alive flag is used to add the Connection and Keep-Alive headers.
 * @return bool - true if sending the directory succeeded, false otherwise.
 */
bool send_directory_listing(int client_socket, char *directory_path, char *document_root, bool keep_alive);

/**
 * Loop through the directory and build the response body.
 *
 * @param response
 * @param directory_path Absolute path to the directory that should be sent.
 * @param document_root Document root is used to check if parent traversal is allowed.
 */
void build_directory_listing_response(http_response *response, char *directory_path, char *document_root);

#endif //NOVAWEB_DIRECTORY_H
