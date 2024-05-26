#include <stdarg.h>
#include <stdio.h>

int count_even(int, ...);

int main()
{
    int n = 5;
    int count = count_even(n, 1, 2, 3, 4, 5);
    printf("Number of even numbers: %d\n", count);
    return 0;
}

int count_even(int n, ...)
{
    int count = 0;
    int i;
    va_list args;
    va_start(args, n);
    for (i = 0; i < n; i++)
    {
        int num = va_arg(args, int);
        if (num % 2 == 0)
        {
            count++;
        }
    }
    va_end(args);
    return count;
}
