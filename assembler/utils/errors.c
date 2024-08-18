/**
 * All the function for constructing and displaying error messages.
 */

#include "../headers/errors.h"
#include <stdio.h>

/**
 * Display error the raised from the assembler file.
 */

void display_error(char *line, int line_number, const char *error_message, const char *file_name)
{
    printf("ERROR: file %s, at \"%s\" (line %d): %s\n", file_name, line, line_number, error_message);
}

/**
 * Display warning the raised from the assembler file.
 */

void display_warning(char *line, int line_number, const char *warning_message, const char *file_name)
{
    printf("WARNING: file %s, at \"%s\" (line %d): %s\n", file_name, line, line_number, warning_message);
}

/**
 * Display error that connected to the internal system.
 */

void display_system_error(const char *error_message, const char *file_name)
{
    printf("ERROR: file %s: %s\n", file_name, error_message);
}
