#include "data.h"

int main()
{
    int a[size];
    int sum, i;
    sum = 0;

    printf("Enter %d numbers: ", size);
    for (i = 0; i < size; i++)
    {
        scanf("%d", &a[i]);
    }

    for (i = 0; i < size; i++)
    {
        sum += a[i];
    }

    printf("Sum of numbers = %d\n", sum);
    
    return 0;
}