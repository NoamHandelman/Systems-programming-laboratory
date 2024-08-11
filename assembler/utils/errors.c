#include "../headers/errors.h"

/**
 * @brief Display a detailed error message with the line number and the line itself.
 * @param line The line that caused the error.
 * @param line_number The line number where the error occurred.
 * @param error_message The error message to display.
 * @param file_name The name of the file that caused the error.
 */

void display_error(char *line, int line_number, const char *error_message, const char *file_name)
{
    printf("ERROR: file %s, at \"%s\" (line %d): %s\n", file_name, line, line_number, error_message);
}

/**
 * @brief Display a detailed warning message with the line number and the line itself.
 * @param line The line that caused the warning.
 * @param line_number The line number where the warning occurred.
 * @param warning_message The warning message to display.
 * @param file_name The name of the file that caused the warning.
 */

void display_warning(char *line, int line_number, const char *warning_message, const char *file_name)
{
    printf("WARNING: file %s, at \"%s\" (line %d): %s\n", file_name, line, line_number, warning_message);
}

void display_system_error(const char *error_message, const char *file_name)
{
    printf("ERROR: file %s: %s\n", file_name, error_message);
}
