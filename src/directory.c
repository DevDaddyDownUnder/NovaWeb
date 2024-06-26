#include <dirent.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "directory.h"
#include "config.h"
#include "http_status.h"
#include "response.h"

// Send a list of files and directories in the specified path to the client.
bool send_directory_listing(int client_socket, char *directory_path, char *document_root, bool keep_alive)
{
    // Build response
    http_response response = {0};
    response.status_code = OK;
    add_response_header(&response, "Content-Type", "text/html");

    // Add Connection header
    if (keep_alive)
    {
        add_response_header(&response, "Connection", "Keep-Alive");

        // Add Keep-Alive header
        char keep_alive_buffer[MAX_HEADER_VALUE_LENGTH];
        snprintf(keep_alive_buffer, sizeof(keep_alive_buffer), "timeout=%d, max=%d", KEEP_ALIVE_TIMEOUT_SECONDS,
                 KEEP_ALIVE_MAX_REQUESTS);
        add_response_header(&response, "Keep-Alive", keep_alive_buffer);
    }
    else
    {
        add_response_header(&response, "Connection", "close");
    }

    // Build directory listing HTML
    build_directory_listing_response(&response, directory_path, document_root);

    // Add content length header
    char file_size_buffer[MAX_HEADER_VALUE_LENGTH];
    snprintf(file_size_buffer, sizeof(file_size_buffer), "%ld", strlen(response.body));
    add_response_header(&response, "Content-Length", file_size_buffer);

    // Build HTTP response
    char output[MAX_RESPONSE_LENGTH];
    build_http_response(&response, output);
    if (verbose_flag)
    {
        print_response(response);
    }

    // Send the response
    size_t response_length = strlen(output);
    ssize_t bytes_sent = send(client_socket, output, response_length, 0);
    if (bytes_sent < (ssize_t) response_length)
    {
        perror("Error sending response headers");
        return false;
    }

    return true;
}

// Loop through the directory and build the response body.
void build_directory_listing_response(http_response *response, char *directory_path, char *document_root)
{
    // Check if we are in the document root
    int skip_parent = 0;
    if (strcmp(directory_path, document_root) == 0)
    {
        skip_parent = 1;
    }

    // Append trailing slash for directory
    strcat(directory_path, "/");

    DIR *dir = opendir(directory_path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    // Build HTML header
    snprintf(response->body, MAX_BODY_LENGTH,
             "<!DOCTYPE html>"
             "<html><head><title>Directory Listing</title></head>"
             "<body><h1>Directory Listing</h1>"
             "<table><thead><th>Name</th><th>Last modified</th><th>Size</th><tbody>");

    // Iterate over directory entries and append HTML links
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip ".", we don't need to list the directory we are already in
        if (strcmp(entry->d_name, ".") == 0)
        {
            continue;
        }

        // We are in the document root, no need to display parent traversal
        if (skip_parent && strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Skip files starting with ".", typically these are hidden files
        if (strcmp(entry->d_name, "..") != 0 && strncmp(".", entry->d_name, strlen(".")) == 0)
        {
            continue;
        }

        // Build absolute file path
        char file_path[FILE_BUFFER_SIZE];
        snprintf(file_path, FILE_BUFFER_SIZE, "%s/%s", directory_path, entry->d_name);
        struct stat st = {0};

        if (stat(file_path, &st) != 0)
        {
            perror("Error getting file information");
            closedir(dir);
            return;
        }

        // Last modified time
        char last_modified[30];
        struct tm *tm_info;
        tm_info = localtime(&st.st_mtime);
        strftime(last_modified, sizeof(last_modified), "%Y-%m-%d %H:%M:%S %Z", tm_info);
        char listing_buffer[FILE_BUFFER_SIZE];

        // Check if it's a directory
        if (S_ISDIR(st.st_mode))
        {
            snprintf(listing_buffer, FILE_BUFFER_SIZE,
                     "<tr><td><a href=\"./%s/\">%s/</a></td><td>%s</td><td>%s</td></tr>",
                     entry->d_name,
                     entry->d_name,
                     last_modified,
                     "-");
        }
        else
        {
            // File size in bytes
            off_t file_size = st.st_size;
            snprintf(listing_buffer, FILE_BUFFER_SIZE,
                     "<tr><td><a href=\"./%s\">%s</a></td><td>%s</td><td>%ld</td></tr>",
                     entry->d_name,
                     entry->d_name,
                     last_modified,
                     (long) file_size);
        }

        strcat(response->body, listing_buffer);
    }

    // Add HTML footer
    strcat(response->body, "</tbody></table></body></html>");

    // Close directory
    closedir(dir);
}
