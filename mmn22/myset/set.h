#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #ifndef H_SET
// #define H_SET

#define SET_SIZE 128

typedef struct
{
    unsigned char data[SET_SIZE / 8];
} set;

void set_init(set *);
void set_read(set *, const char *);
void set_print(const set *);
void set_union(set *, const set *, const set *);
void set_intersect(set *, const set *, const set *);
void set_subtract(set *, const set *, const set *);
void set_symdiff(set *, const set *, const set *);

// #endif
