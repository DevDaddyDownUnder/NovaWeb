#ifndef NOVAWEB_DIRECTORY_H
#define NOVAWEB_DIRECTORY_H

#include "file.h"
#include "response.h"

void send_directory_listing(int client_socket, char *directory_path, char *document_root);
void build_directory_listing_response(http_response *response, char *directory_path, char *document_root);

#endif //NOVAWEB_DIRECTORY_H
