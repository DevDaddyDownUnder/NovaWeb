cmake_minimum_required(VERSION 3.27)
project(NovaWeb C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-equal")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-enum")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")

add_executable(NovaWeb src/main.c
        src/http_server.c
        src/request.c
        src/response.c
        src/path.c
        src/virtual_host.c
        src/file.c
        src/mime_types.c
        src/http_status.c
        src/config.c
        src/directory.c
        src/url.c
        src/string.c)
