#ifndef NOVAWEB_PATH_H
#define NOVAWEB_PATH_H

#define DEFAULT_FILE "index.html"
#define DEFAULT_FILE_EXTENSION ".html"

/**
 * Normalize the file path, replacing back slashes, removes redundant slashes and null-terminates.
 *
 * @param path
 */
void normalize_path(char *path);

/**
 * Prevent directory traversal, set default file if root directory is requested.
 *
 * @param requested_file
 * @return The normalized file path.
 */
int build_file_path(char *requested_file);

#endif //NOVAWEB_PATH_H
