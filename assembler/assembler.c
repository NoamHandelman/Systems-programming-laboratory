#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *am_file;

    while (--argc > 0)
    {
        exec_preproc(argv[argc]);
    }
}