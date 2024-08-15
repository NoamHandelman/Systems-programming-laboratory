#ifndef STRINGS_H
#define STRINGS_H

/**
 * @brief Check for extra forbidden characters in the line.
 * @param end_token The end of the line.
 * @return 1 if there are extra characters, 0 otherwise.
 */

int check_for_extra_chars(char *end_token);

/**
 * @brief Check if the line is empty.
 * @param line The line to check.
 */

int is_empty_line(const char *line);

/**
 * @brief The function that centralizes space handling for the line.
 * @param line The line to handle.
 */

void handle_spaces(char *line);

#endif
