# Compiler
CC = gcc

# Compiler flags
STD = -std=c11
CFLAGS = -Wall \
 		 -Werror \
 		 -Wextra \
 		 -Wuninitialized \
 		 -Wunused \
 		 -Wconversion \
 		 -Wformat \
 		 -Wmissing-prototypes \
 		 -Wshadow \
 		 -Wsign-compare \
 		 -Wfloat-equal \
 		 -Wpointer-arith \
 		 -Wswitch-enum \
 		 -pedantic \
 		 -D_GNU_SOURCE # GNU extension required for strsep function in request.c

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
	src/url.c \
	src/string.c

# Object files
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Executable name
TARGET = build/NovaWeb

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(STD) -o $@ $^

# Rule to compile source files
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(STD) -c -o $@ $<

# Rule to create the build directory
build:
	mkdir -p build

# Install the binary
install:
	cp $(TARGET) /usr/local/bin/novaweb

# Clean target to remove object files and executable
clean:
	rm -rf build