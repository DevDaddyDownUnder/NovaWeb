#ifndef NOVAWEB_PATH_H
#define NOVAWEB_PATH_H

#define DEFAULT_FILE "index.html"
#define DEFAULT_FILE_EXTENSION ".html"

void normalize_path(char *path);

int build_file_path(char *requested_file);

#endif //NOVAWEB_PATH_H
