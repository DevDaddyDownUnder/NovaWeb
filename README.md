# NovaWeb
A basic HTTP Server written in C.


## Disclaimer

This server is intended for local development only and should **NOT** be used in a production environment or accessible outside your own network.


## Features


## Usage

NovaWeb has a default port of 8080 and will serve files from the directory it's ran from.

```bash
NovaWeb
```

Start NovaWeb on a specified port:

```bash
NovaWeb -p 8081
```

Start NovaWeb with a specified port, host and document root:

```bash
NovaWeb -p 8081 -h 127.0.0.1 -d /var/www/sites/example
```

### Options

| Argument  | Name              | Description                                                                            |
|-----------|:------------------|:---------------------------------------------------------------------------------------|
| -p        | port              | The port number to bind to. Default value is 8080.                                     |
| -h        | host              | The virtual host to route requests to the correct document root.                       |
| -d        | document root     | The document root of your files to be served.                                          |
| --dl      | directory listing | The directory listing flag enables directory listing. Default value is false.          |
| --mp      | multi process     | The multi process flag will create a new worker for each core on the cpu.              |
| --ss      | server signature  | The server signature flag disables the server signature header. Default value is true. |
| --verbose | verbose           | The verbose flag enables detailed logging of request and response objects.             |


## Compiling

To build the binary yourself, follow the documentation matching your operating system.

### macOS / Linux

Open a terminal and navigate to the project root.

Create a build directory:
```bash
mkdir build
```

From the root of the project run the make command:
```bash
make
```

If there were no errors the binary will be available in the build folder.

```bash
./build/NovaWeb
```

## Install

To install the binary, follow the documentation matching your operating system.

### macOS

Copy the binary to the /usr/local/bin directory.

```bash
make install
```
