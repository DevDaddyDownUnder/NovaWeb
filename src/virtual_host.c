#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "virtual_host.h"

host_config hosts[MAX_HOSTS];
int sizeof_hosts = 0;

// Add a new host config.
void add_host_config(char *host, char *document_root)
{
    if (sizeof_hosts > MAX_HOSTS)
    {
        fprintf(stderr, "Maximum number of hosts (%d) reached.\n", MAX_HOSTS);
        return;
    }

    strncpy(hosts[sizeof_hosts].host, host, MAX_HOST_LENGTH - 1);
    strncpy(hosts[sizeof_hosts].document_root, document_root, MAX_DOCUMENT_ROOT_LENGTH - 1);
    sizeof_hosts++;
}

// Return the document root for the specified host.
char *get_document_root(char host[MAX_HOST_LENGTH])
{
    for (int i = 0; i < sizeof_hosts; i++)
    {
        if (strcmp(host, hosts[i].host) == 0)
        {
            return hosts[i].document_root;
        }
    }

    // Fallback to the directory where the program was executed
    static char cwd[MAX_DOCUMENT_ROOT_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        return cwd;
    }

    perror("Unable to fallback to the directory where the program was executed. getcwd() error.");
    return "";
}
