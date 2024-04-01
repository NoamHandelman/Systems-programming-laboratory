#include <stdio.h>

int main()
{
    int sum, num;

    sum = 0;

    printf("Enter numbers: ");

    while (scanf("%d", &num) != EOF)
    {
        sum += num;
    }

    printf("Sum of numbers = %d\n", sum);

    return 0;
}