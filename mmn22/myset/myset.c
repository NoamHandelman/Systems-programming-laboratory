#include "set.h"

void init_set(set *s)
{
    int i;
    for (i = 0; i < SET_SIZE / 8; i++)
        s->data[i] = 0;
}

void read_set(set *s, int numbers[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        s->data[numbers[i] / 8] |= 1 << (numbers[i] % 8);
    }
}

void print_set(const set *s)
{
    int i, count = 0;
    int empty = 1;
    for (i = 0; i < SET_SIZE; i++)
    {
        if (s->data[i / 8] & (1 << (i % 8)))
        {
            if (count > 0 && count % 16 == 0)
            {
                printf("\n");
            }
            printf("%d ", i);
            count++;
            empty = 0;
        }
    }
    if (empty)
    {
        printf("The set is empty");
    }
    printf("\n");
}
