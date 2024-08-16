/**
 * The file that responsible for the string manipulation functions.
 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "../headers/strings.h"
#include "../headers/globals.h"

/**
 * @brief Remove the whitespace from the edges of the string.
 * @param line The line to remove the whitespace from.
 */

void remove_whitespace_from_edges(char *line);

/**
 * @brief Add spaces between the commas in lines that are not .string directive. to simplify line parsing later.
 * @param line The line to add spaces to.
 */
void add_spaces(char *line);

int check_for_extra_chars(char *end_token)
{
    while (*end_token != '\0')
    {
        if (!isspace(*end_token))
        {
            return 1;
        }
        end_token++;
    }
    return 0;
}

int is_empty_line(const char *line)
{
    while (*line != '\0')
    {
        if (!isspace(*line))
        {
            return 0;
        }
        line++;
    }
    return 1;
}

void remove_whitespace_from_edges(char *line)
{
    char *start = line;
    char *end = NULL;

    /**
     * Remove the whitespace from the start of the line.
     */

    if (*start != '\"')
    {
        while (isspace((unsigned char)*start))
            start++;
    }

    end = start + strlen(start) - 1;

    /**
     * Remove the whitespace from the end of the line.
     */

    if (*end != '\"')
    {
        while (end > start && isspace((unsigned char)*end))
            end--;
    }
    *(end + 1) = '\0';

    /**
     * Move the line to the start of the string.
     * The +2 is to include the null terminator.
     */

    memmove(line, start, end - start + 2);
}

void add_spaces(char *line)
{
    /**
     * 
     */
    char result[INITIAL_BUFFER_SIZE];
    int j = 0, i;
    int in_quotes = 0;

    /**
     * If the line is .entry or .extern directive, we don't need to add spaces.
     */

    if (strstr(line, ".entry") || strstr(line, ".extern"))
        return;

    for (i = 0; line[i] != '\0'; i++)
    {
        /**
         * Check if we are in quotes to avoid adding spaces between the commas in the .string directive.
         */
        if (line[i] == '\"')
        {
            in_quotes = !in_quotes;
        }

        /**
         * Add spaces between the commas.
         */

        if (!in_quotes && line[i] == ',')
        {
            result[j++] = ' ';
            result[j++] = ',';
            result[j++] = ' ';
        }
        else
        {
            result[j++] = line[i];
        }
    }

    result[j] = '\0';
    strcpy(line, result);
}

void handle_spaces(char *line)
{
    remove_whitespace_from_edges(line);
    add_spaces(line);
}
