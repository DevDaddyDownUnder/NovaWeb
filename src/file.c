#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "file.h"
#include "path.h"
#include "response.h"
#include "http_status.h"
#include "mime_types.h"
#include "config.h"

// Send the file to the client.
void send_file(int client_socket, char *file_path)
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
            // TODO at this point I should send a 404
            perror("Error opening requested file");
            close(client_socket);
            pthread_exit(NULL);
        }
    }

    // Get file size
    struct stat st;
    if (fstat(fileno(file_ptr), &st) < 0)
    {
        // TODO send a 500?
        perror("Error getting file size.");
        fclose(file_ptr);
        close(client_socket);
        pthread_exit(NULL);
    }
    off_t file_size = st.st_size;
    char file_size_buffer[MAX_HEADER_VALUE_LENGTH];
    snprintf(file_size_buffer, sizeof(file_size_buffer), "%lld", file_size);

    // Build response
    http_response response;
    memset(&response, 0, sizeof(http_response));
    response.status_code = OK;
    get_status_message(response.status_code, response.status_message, sizeof(response.status_message));

    // Get mime type of the file
    char mime_type[MAX_MIME_TYPE_LENGTH];
    get_mime_type(file_path ,mime_type, sizeof(mime_type));
    add_header(&response, "Content-Type", mime_type);

    // Add content length header
    add_header(&response, "Content-Length", file_size_buffer);

    // Add Transfer-Encoding header
//    add_header(&response, "Transfer-Encoding", "chunked");

    // TODO add a flag to toggle adding this header or not
    // TODO define NovaWeb label and version somewhere
    // Add server header
    add_header(&response, "Server", "NovaWeb/0.1");

    char output[MAX_RESPONSE_LENGTH];
    build_http_response(&response, output);
    if (verbose_flag)
    {
        print_response(response);
    }

    // Send the response
    size_t response_length = strlen(output);
    size_t bytes_sent = send(client_socket, output, response_length, 0);
    if (bytes_sent < response_length)
    {
        // TODO send a 500?
        perror("Error sending response headers");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Send the file contents
    send_file_contents(client_socket, file_ptr);
}

// Read the files contents and send it to the client
void send_file_contents(int client_socket, FILE *file_ptr)
{
    char buffer[FILE_BUFFER_SIZE];
    size_t bytes_read, bytes_sent;

    while ((bytes_read = fread(buffer, 1, FILE_BUFFER_SIZE, file_ptr)) > 0)
    {
        size_t total_sent = 0;
        while (total_sent < bytes_read)
        {
            bytes_sent = send(client_socket, buffer + total_sent, bytes_read - total_sent, 0);
            if (bytes_sent <= 0)
            {
                perror("Error sending file contents");
                fclose(file_ptr);
                close(client_socket);
                pthread_exit(NULL);
            }
            total_sent += bytes_sent;
        }
    }

    // Close file pointer
    fclose(file_ptr);
}

// Read the files contents and send it to the client
void send_chunked_file_contents(int client_socket, FILE *file_ptr)
{
    char buffer[FILE_BUFFER_SIZE];
    size_t bytes_read;
    size_t bytes_sent;

    while ((bytes_read = fread(buffer, 1, FILE_BUFFER_SIZE, file_ptr)) > 0)
    {
        if (send_chunk(client_socket, buffer, bytes_read) == 0)
        {
            fclose(file_ptr);
            close(client_socket);
            pthread_exit(NULL);
        }
    }

    // Send zero-size chunk to indicate end of file
    if (send(client_socket, "0\r\n\r\n", 5, 0) < 0)
    {
        perror("Error sending zero-size chunk");
    }

    // Close file pointer
    fclose(file_ptr);
}

// Send a chunk of data.
int send_chunk(int client_socket, char *buffer, size_t size)
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
        return 0;
    }

    // Send chunk data
    while (total_sent < size)
    {
        bytes_sent = send(client_socket, buffer + total_sent, size - total_sent, 0);
        if (bytes_sent <= 0)
        {
            perror("Error sending file contents");
            return 0;
        }

        total_sent += bytes_sent;
    }

    // Send end of chunk marker
    bytes_sent = send(client_socket, "\r\n", 2, 0);
    if (bytes_sent <= 0)
    {
        perror("Error sending chunk end marker");
        return 0;
    }

    return 1;
}
