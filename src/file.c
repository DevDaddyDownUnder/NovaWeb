#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include "file.h"
#include "config.h"
#include "http_status.h"
#include "mime_types.h"
#include "path.h"
#include "response.h"

// Send the file to the client.
bool send_file(int client_socket, char *file_path, bool keep_alive)
{
    // Open requested file
    FILE *file_ptr = fopen(file_path, "r");
    if (file_ptr == NULL)
    {
        // Add default extension if missing
        if (strcmp(file_path, "/") != 0 && strchr(file_path, '.') == NULL)
        {
            strcat(file_path, DEFAULT_FILE_EXTENSION);
            // Attempt to open file with default extension
            file_ptr = fopen(file_path, "r");
        }
        if (file_ptr == NULL)
        {
            send_not_found(client_socket);
            return false;
        }
    }

    // Get file size
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    if (stat(file_path, &st) != 0)
    {
        send_not_found(client_socket);
        return false;
    }

    // Build response
    http_response response;
    memset(&response, 0, sizeof(http_response));
    response.status_code = OK;

    // Get mime type of the file
    char *mime_type = get_mime_type(file_path);
    add_response_header(&response, "Content-Type", mime_type);

    // Add content length header
    off_t file_size = st.st_size;
    char file_size_buffer[MAX_HEADER_VALUE_LENGTH];
    snprintf(file_size_buffer, sizeof(file_size_buffer), "%ld", (long)file_size);
    add_response_header(&response, "Content-Length", file_size_buffer);

    // Add last modified header
    char last_modified_buffer[30];
    struct tm tm_info;
    gmtime_r(&st.st_mtime, &tm_info);
    strftime(last_modified_buffer, sizeof(last_modified_buffer), "%Y-%m-%d %H:%M:%S %Z", &tm_info);
    add_response_header(&response, "Last-Modified", last_modified_buffer);

    // Add Connection header
    if (keep_alive)
    {
        add_response_header(&response, "Connection", "Keep-Alive");

        // TODO only need to create this header once
        // Add Keep-Alive header
        char keep_alive_buffer[MAX_KEEP_ALIVE_LENGTH];
        snprintf(keep_alive_buffer, sizeof(keep_alive_buffer), "timeout=%d, max=%d", KEEP_ALIVE_TIMEOUT_SECONDS, KEEP_ALIVE_MAX_REQUESTS);
        add_response_header(&response, "Keep-Alive", keep_alive_buffer);
    }
    else
    {
        add_response_header(&response, "Connection", "close");
    }

    char output[MAX_RESPONSE_LENGTH];
    build_http_response(&response, output);
    if (verbose_flag)
    {
        print_response(response);
    }

    // Send the response
    size_t response_length = strlen(output);
    ssize_t bytes_sent = send(client_socket, output, response_length, 0);
    if (bytes_sent < (ssize_t)response_length)
    {
        perror("Error sending response headers");
        return false;
    }

    // Send the file contents
    return send_file_contents(client_socket, file_ptr);
}

// Read the files contents and send it to the client
bool send_file_contents(int client_socket, FILE *file_ptr)
{
    char buffer[FILE_BUFFER_SIZE];
    size_t bytes_read;
    ssize_t bytes_sent;

    while ((bytes_read = fread(buffer, 1, FILE_BUFFER_SIZE, file_ptr)) > 0)
    {
        ssize_t total_sent = 0;
        while (total_sent < (ssize_t)bytes_read)
        {
            bytes_sent = send(client_socket, buffer + total_sent, bytes_read - (size_t)total_sent, 0);
            if (bytes_sent <= 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // Socket buffer is full, try again later
                    continue;
                }

                perror("Error sending file contents");
                fclose(file_ptr);
                return false;
            }
            total_sent += bytes_sent;
        }
    }

    // Close file pointer
    fclose(file_ptr);

    return true;
}
