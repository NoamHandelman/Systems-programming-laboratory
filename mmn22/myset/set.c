#include "set.h"

void init_sets(set sets[], int size)
{
    int i;
    int j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < SET_SIZE / 8; j++)
        {
            sets[i].data[j] = 0;
        }
    }
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
    int count = 0;
    int empty = 1;
    int i;
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

void union_set(set *result, const set *s1, const set *s2)
{
    int i;
    for (i = 0; i < SET_SIZE / 8; i++)
    {
        result->data[i] = s1->data[i] | s2->data[i];
    }
}

void intersect_set(set *result, const set *s1, const set *s2)
{
    int i;
    for (i = 0; i < SET_SIZE / 8; i++)
    {
        result->data[i] = s1->data[i] & s2->data[i];
    }
}

void subtract_set(set *result, const set *s1, const set *s2)
{
    int i;
    for (i = 0; i < SET_SIZE / 8; i++)
    {
        result->data[i] = s1->data[i] & ~s2->data[i];
    }
}

void symdiff_set(set *result, const set *s1, const set *s2)
{
    int i;
    for (i = 0; i < SET_SIZE / 8; i++)
    {
        result->data[i] = s1->data[i] ^ s2->data[i];
    }
}
