#ifndef NOVAWEB_STRING_H
#define NOVAWEB_STRING_H

/**
 * Remove space from left of string.
 *
 * @param string
 * @return Left trimmed string.
 */
char *ltrim(char *string);

/**
 * Remove space from right of string.
 *
 * @param string
 * @return Right trimmed string.
 */
char *rtrim(char *string);

/**
 * Trim left and right space from string.
 *
 * @param string
 * @return Left and right trimmed string.
 */
char *trim(char *string);

#endif //NOVAWEB_STRING_H
