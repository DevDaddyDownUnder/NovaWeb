# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Source files
SRCS = \
	src/main.c \
	src/http_server.c \
	src/request.c \
	src/response.c \
	src/path.c \
	src/virtual_host.c \
	src/file.c \
	src/mime_types.c \
	src/http_status.c \
	src/config.c \
	src/directory.c \
	src/url.c

# Object files
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Executable name
TARGET = build/NovaWeb

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c -o $@ $<

# Install the binary
install:
	cp $(TARGET) /usr/local/bin/novaweb

# Clean target to remove object files and executable
clean:
	rm -f build