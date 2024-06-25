#include "../headers/strings.h"
#include <stdio.h>

int check_for_extra_chars(char *token_end)
{
    printf("Checking for extra characters after: '%s'\n", token_end);
    while (*token_end != '\0')
    {
        printf("token_end: %c\n", *token_end);
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
