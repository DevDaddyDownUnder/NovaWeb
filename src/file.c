#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "file.h"
#include "config.h"
#include "http_status.h"
#include "mime_types.h"
#include "path.h"
#include "response.h"

// Send the file to the client.
bool send_file(int client_socket, char *file_path)
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

    off_t file_size = st.st_size;
    char file_size_buffer[MAX_HEADER_VALUE_LENGTH];
    snprintf(file_size_buffer, sizeof(file_size_buffer), "%ld", (long)file_size);

    // Build response
    http_response response;
    memset(&response, 0, sizeof(http_response));
    response.status_code = OK;

    // Get mime type of the file
    char *mime_type = get_mime_type(file_path);
    add_response_header(&response, "Content-Type", mime_type);

    // Add content length header
    add_response_header(&response, "Content-Length", file_size_buffer);

    // Add Transfer-Encoding header
//    add_header(&response, "Transfer-Encoding", "chunked");

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

// Read the files contents and send it to the client
bool send_chunked_file_contents(int client_socket, FILE *file_ptr)
{
    char buffer[FILE_BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, FILE_BUFFER_SIZE, file_ptr)) > 0)
    {
        if (send_chunk(client_socket, buffer, bytes_read) == 0)
        {
            fclose(file_ptr);
            return false;
        }
    }

    // Send zero-size chunk to indicate end of file
    if (send(client_socket, "0\r\n\r\n", 5, 0) < 0)
    {
        perror("Error sending zero-size chunk");
    }

    // Close file pointer
    fclose(file_ptr);

    return true;
}

// Send a chunk of data.
bool send_chunk(int client_socket, char *buffer, size_t size)
{
    char chunk_header[20];
    ssize_t bytes_sent;
    ssize_t total_sent = 0;

    // Generate chunk header in hexadecimal format
    sprintf(chunk_header, "%zx\r\n", size);

    // Send chunk header
    bytes_sent = send(client_socket, chunk_header, strlen(chunk_header), 0);
    if (bytes_sent <= 0)
    {
        perror("Error sending chunk header");
        return false;
    }

    // Send chunk data
    while ((size_t)total_sent < size)
    {
        bytes_sent = send(client_socket, buffer + total_sent, size - (size_t)total_sent, 0);
        if (bytes_sent <= 0)
        {
            perror("Error sending file contents");
            return false;
        }

        total_sent += bytes_sent;
    }

    // Send end of chunk marker
    bytes_sent = send(client_socket, "\r\n", 2, 0);
    if (bytes_sent <= 0)
    {
        perror("Error sending chunk end marker");
        return false;
    }

    return true;
}
