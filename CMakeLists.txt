cmake_minimum_required(VERSION 3.27)
project(NovaWeb C)

set(CMAKE_C_STANDARD 11)

add_executable(NovaWeb src/main.c
        src/http_server.c
        src/request.c
        src/response.c
        src/path.c
        src/virtual_host.c
        src/file.c
        src/mime_types.c
        src/http_status.c)
