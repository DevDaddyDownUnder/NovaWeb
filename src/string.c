#include <ctype.h>
#include <string.h>
#include "string.h"

// Remove space from left of string.
char *ltrim(char *string)
{
    while (isspace(*string))
    {
        string++;
    }

    return string;
}

// Remove space from right of string.
char *rtrim(char *string)
{
    char *end = string + strlen(string) -1;

    while (end > string && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    return string;
}

// Trim left and right space from string.
char *trim(char *string)
{
    return rtrim(ltrim(string));
}
