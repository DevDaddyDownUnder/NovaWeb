cmake_minimum_required(VERSION 3.27)
project(NovaWeb C)

set(CMAKE_C_STANDARD 11)

add_executable(NovaWeb src/main.c
        src/http_server.c)
