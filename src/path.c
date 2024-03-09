#include <string.h>
#include "path.h"

void normalize_path(char *path)
{
    size_t length = strlen(path);

    // Replace backslashes with forward slashes
    size_t i;
    for (i = 0; i < length; i++)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }

    // Remove redundant slashes and trailing slash (except for root directory)
    size_t j = 0;
    for (i = 0; i < length; i++)
    {
        if (!(path[i] == '/' && (i + 1 < length && path[i + 1] == '/')))
        {
            path[j++] = path[i];
        }
    }

    // Ensure the path is null-terminated
    if (j > 1 && path[j - 1] == '/')
    {
        j--;
    }

    path[j] = '\0';
}

int build_file_path(char *file)
{
    // Normalize the file path
    normalize_path(file);

    // Validate requested file path, prevent directory traversal
    if (strstr(file, "..") != NULL)
    {
        return 0;
    }

    // If root directory is requested, serve index.html
    if (strcmp(file, "/") == 0)
    {
        strcpy(file, DEFAULT_FILE);
    }
    else
    {
        // Remove leading '/'
        memmove(file, file + 1, strlen(file));
    }

    return 1;
}
