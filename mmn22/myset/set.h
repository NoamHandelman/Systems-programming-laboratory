#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_SIZE 128
#define SETS_NUMBER 6

typedef struct
{
    unsigned char data[SET_SIZE / 8];
} set;

void init_sets(set sets[], int size);
void read_set(set *, int[], int);
void print_set(const set *);
void union_set(set *, const set *, const set *);
void intersect_set(set *, const set *, const set *);
void subtract_set(set *, const set *, const set *);
void symdiff_set(set *, const set *, const set *);

#endif
