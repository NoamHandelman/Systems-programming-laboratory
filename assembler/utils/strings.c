#include "../headers/strings.h"

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

void trim_whitespace(char *line)
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

void add_spaces(char *str)
{
    char result[1024] = {0};
    int j = 0, i;
    int in_quotes = 0;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\"')
        {
            in_quotes = !in_quotes;
        }

        if (!in_quotes && str[i] == ':')
        {
            while (j > 0 && isspace((unsigned char)result[j - 1]))
            {
                j--;
            }
            result[j++] = ':';
            result[j++] = ' ';
        }
        else if (!in_quotes && str[i] == ',')
        {
            result[j++] = ' ';
            result[j++] = ',';
            result[j++] = ' ';
        }
        else
        {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    strcpy(str, result);
}

void remove_extra_spaces(char *str)
{
    char result[1024] = {0};
    int j = 0, i;
    int space_found = 0;
    int in_quotes = 0;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\"')
        {
            in_quotes = !in_quotes;
        }

        if (!in_quotes && isspace((unsigned char)str[i]))
        {
            if (!space_found)
            {
                result[j++] = ' ';
                space_found = 1;
            }
        }
        else
        {
            result[j++] = str[i];
            space_found = 0;
        }
    }
    result[j] = '\0';
    strcpy(str, result);
}

void handle_spaces(char *line)
{
    trim_whitespace(line);
    add_spaces(line);
    remove_extra_spaces(line);
}
