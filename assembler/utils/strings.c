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
