#include "set.h"

int main()
{
    int numbers[] = {4, 8, 12, 16, 20, 127, 13, 18, 4, 99, 100, 111, 66, 44, 77, 99, 117, 127, 1, 2, 3};

    set SETA, SETB, SETC, SETD, SETE, SETF;
    init_set(&SETA);
    init_set(&SETB);
    init_set(&SETC);
    init_set(&SETD);
    init_set(&SETE);
    init_set(&SETF);

    read_set(&SETA, numbers, 21);

    print_set(&SETA);

    return 0;
}
