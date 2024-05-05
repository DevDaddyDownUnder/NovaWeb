#ifndef HTTPSERVER_URL_H
#define HTTPSERVER_URL_H

/**
 * Decode a URL encoded string.
 *
 * @param encoded
 * @param decoded
 * @param max_length
 */
void url_decode(const char *encoded, char *decoded, size_t max_length);

#endif //HTTPSERVER_URL_H
