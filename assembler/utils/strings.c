#include "../headers/strings.h"

void remove_whitespace_from_edges(char *);
void add_spaces(char *);
void remove_extra_spaces(char *);

int check_for_extra_chars(char *token_end)
{
    while (*token_end != '\0')
    {
        if (!isspace(*token_end))
        {
            return 1;
        }
        token_end++;
    }
    return 0;
}

int is_empty_line(const char *line)
{
    while (*line != '\0')
    {
        if (!isspace(*line))
        {
            if (*line == ';')
            {
                return 1;
            }
            return 0;
        }
        line++;
    }
    return 1;
}

/**
 * void remove_spaces(char *line)
{
    char *start;
    char *end;

    start = line;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }

    if (*start == '\0')
    {
        line[0] = '\0';
        return;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = '\0';

    memmove(line, start, end - start + 2);
}
 */

void remove_whitespace_from_edges(char *line)
{
    char *start = line;
    char *end;

    if (*start != '\"')
    {
        while (isspace((unsigned char)*start))
            start++;
    }

    end = start + strlen(start) - 1;

    if (*end != '\"')
    {
        while (end > start && isspace((unsigned char)*end))
            end--;
    }
    *(end + 1) = '\0';

    memmove(line, start, end - start + 2);
}

void add_spaces(char *line)
{
    char result[1024] = {0};
    int j = 0, i;
    int in_quotes = 0;

    for (i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '\"')
        {
            in_quotes = !in_quotes;
        }

        if (!in_quotes && line[i] == ':')
        {
            while (j > 0 && isspace((unsigned char)result[j - 1]))
            {
                j--;
            }
            result[j++] = ':';
            result[j++] = ' ';
        }
        else if (!in_quotes && line[i] == ',')
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

void remove_extra_spaces(char *line)
{
    char result[1024] = {0};
    int j = 0, i;
    int space_found = 0;
    int in_quotes = 0;

    for (i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '\"')
        {
            in_quotes = !in_quotes;
        }

        if (!in_quotes && isspace((unsigned char)line[i]))
        {
            if (!space_found)
            {
                result[j++] = ' ';
                space_found = 1;
            }
        }
        else
        {
            result[j++] = line[i];
            space_found = 0;
        }
    }
    result[j] = '\0';
    strcpy(line, result);
}

void handle_spaces(char *line)
{
    remove_whitespace_from_edges(line);
    add_spaces(line);
    remove_extra_spaces(line);
}
