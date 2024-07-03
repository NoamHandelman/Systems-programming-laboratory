#include "../headers/strings.h"
#include <stdio.h>

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

void remove_spaces(char *line)
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
