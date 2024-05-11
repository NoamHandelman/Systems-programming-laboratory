/*
This program reads an array of integers from the user and calculates the partial sums of the array into a new array.
*/

#include <stdio.h>
#include <stdlib.h>

/* A function to print array elements */
void printArray(int *, int);

/* A function to calculate the partial sums of an array */
int *partialSums(int *, int);

void printArray(int *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int *partialSums(int *initialArray, int size)
{
    int sum = 0;
    int i;
    int *partialSum = malloc(size * sizeof(int)); /* Allocate memory for the partial sum array */

    if (partialSum == NULL) /* Check if memory allocation failed */
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (i = 0; i < size; i++) /* Calculate the partial sums */
    {
        sum += initialArray[i];
        partialSum[i] = sum;
    }

    return partialSum;
}

int main()
{
    int size;
    int i;
    int *initialArray;
    int *partialSumArray;

    printf("Please enter the size of the array:\n");
    scanf("%d", &size);
    printf("You entered %d\n", size);

    initialArray = malloc(size * sizeof(int)); /* Allocate memory for the initial array */
    if (initialArray == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter the array elements:\n");
    for (i = 0; i < size; i++)
    {
        printf("Please enter element number %d:\n", i + 1);
        scanf("%d", &initialArray[i]);
        printf("You entered %d\n", initialArray[i]);
    }

    printf("The original array is: ");
    printArray(initialArray, size);

    partialSumArray = partialSums(initialArray, size); /* Calculate the partial sums */

    printf("The partial sums array is: ");
    printArray(partialSumArray, size);

    /* Free the memory allocated for the arrays */
    free(initialArray);
    free(partialSumArray);

    return 0;
}
