#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "url.h"

// Decode a URL encoded string.
void url_decode(const char *encoded, char *decoded, size_t max_length)
{
    size_t encoded_length = strlen(encoded);
    size_t decoded_index = 0;

    for (size_t encoded_index = 0; encoded_index < encoded_length && decoded_index < max_length - 1; encoded_index++)
    {
        if (encoded[encoded_index] == '%')
        {
            if (encoded[encoded_index + 1] && encoded[encoded_index + 2])
            {
                // Extract the two hexadecimal digits following '%'
                char hex_string[3] = {
                        encoded[encoded_index + 1],
                        encoded[encoded_index + 2],
                        '\0'};
                unsigned long hex_value = strtoul(hex_string, NULL, 16);

                if (hex_value != ULONG_MAX)
                {
                    decoded[decoded_index++] = (char) hex_value;
                    // Move past the percent-encoded characters
                    encoded_index += 2;
                }
                else
                {
                    // Error handling for invalid hex string
                    decoded[decoded_index++] = encoded[encoded_index];
                }
            }
            else
            {
                // Incomplete percent-encoded sequence
                decoded[decoded_index++] = encoded[encoded_index];
            }
        }
        else if (encoded[encoded_index] == '+')
        {
            // Replace '+' with space
            decoded[decoded_index++] = ' ';
        }
        else
        {
            // Copy non-encoded characters as is
            decoded[decoded_index++] = encoded[encoded_index];
        }
    }

    // Null-terminate the decoded string
    decoded[decoded_index] = '\0';
}
