#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

/**
 * @brief Display a detailed error message with the line number and the line itself.
 */

void display_error(char *, int, const char *, const char *);

/**
 * @brief Display a detailed warning message with the line number and the line itself.
 */

void display_warning(char *, int, const char *, const char *);

void display_system_error(const char *, const char *);

#endif
