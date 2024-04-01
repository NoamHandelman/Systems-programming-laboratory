#include <stdio.h>
#include <ctype.h>

int main()
{
    int ch;
    int countSmall = 0, countBig = 0;

    printf("Enter a sentence: ");

    while ((ch = getchar()) != EOF) {
        putchar(ch);
        if (islower(ch)) {
            countSmall++;
        } else if (isupper(ch)) {
            countBig++;
        }
    }
    printf("\nNumber of small letters = %d\n", countSmall);
    printf("Number of capital letters = %d\n", countBig);

    return 0;
}