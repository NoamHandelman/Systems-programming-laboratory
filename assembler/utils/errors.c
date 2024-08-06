#include "../headers/errors.h"

/**
 * @brief Display a detailed error message with the line number and the line itself.
 * @param line The line that caused the error.
 * @param line_number The line number where the error occurred.
 * @param error_message The error message to display.
 * @param file_name The name of the file that caused the error.
 */

void display_error(char *line, int line_number, char *error_message, char *file_name)
{
    printf("ERROR: file %s, at %s (line %d): %s\n", file_name, line, line_number, error_message);
}
