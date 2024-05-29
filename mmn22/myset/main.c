#include "set.h"

int main()
{
    char sets[6] = {'SETA', 'SETB', 'SETC', 'SETD', 'SETE', 'SETF'};
    set SETA, SETB, SETC, SETD, SETE, SETF;
    set *set_array[6] = {&SETA, &SETB, &SETC, &SETD, &SETE, &SETF};
    init_sets(set_array, 6);

    return 0;
}
