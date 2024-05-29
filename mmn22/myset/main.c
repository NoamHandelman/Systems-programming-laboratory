#include "set.h"

int get_set_index(const char *, const char[]);

int get_set_index(const char *set_name, const char sets[])
{
    int i;
    for (i = 0; i < SETS_NUMBER; i++)
    {
        if (strcmp(set_name, sets[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    char sets[SETS_NUMBER] = {'SETA', 'SETB', 'SETC', 'SETD', 'SETE', 'SETF'};
    set SETA, SETB, SETC, SETD, SETE, SETF;
    set *set_array[SETS_NUMBER] = {&SETA, &SETB, &SETC, &SETD, &SETE, &SETF};
    init_sets(set_array, SETS_NUMBER);

    return 0;
}
