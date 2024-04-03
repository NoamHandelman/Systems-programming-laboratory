/* Program to print all suffixes starting with some char and return the count of such suffixes */

#include <stdio.h>
#include <string.h>

/* Function to find all the suffix and return the count of such */
int suffix(char[], char);

int suffix(char str[], char c)
{
    int count = 0;         /* counter for suffixes starting with c */
    int len = strlen(str); /* length of string */

    for (int i = 0; i < len; i++)
    {
        if (str[i] == c)
        {
            printf("%s\n", str + i);
            count++;
        }
    }

    return count;
}

int main()
{
    char c, str[128];
    int index = 0;
    char temp;

    printf("Enter a character followed and then a string (max 127 characters): ");
    
    c = getchar();
    
    while((temp = getchar()) != '\n' && temp != EOF && index < 127)
    {
        str[index++] = temp;
    }
    str[index] = '\0';

    printf("You entered: '%c' and \"%s\"\n", c, str);

    int matches = suffix(str, c);

    printf("Number of suffixes starting with '%c': %d\n", c, matches);

    return 0;
}
