#include <stdio.h>
#include <string.h>
#include <ctype.h>

void trim_whitespace(char *str)
{
    char *start = str;
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

    memmove(str, start, end - start + 2);
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

int main()
{
    char input[1024];

    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    trim_whitespace(input);
    add_spaces(input);
    remove_extra_spaces(input);

    printf("Processed string: %s\n", input);

    return 0;
}
