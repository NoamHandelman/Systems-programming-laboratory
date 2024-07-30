#ifndef FILES_H
#define FILES_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * @brief Create a file path with the given filename and extension.
 * @return the newly created file path.
 */
char *create_file(const char *, const char *);

#endif
