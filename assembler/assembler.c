#include "headers/pre_proc.h"

int main(int argc, char *argv[])
{
    int i;

    printf("argc: %d\n", argc);

    if (argc < 2)
    {
        fprintf(stderr, "No input files\n");
    }

    for (i = 1; i < argc; i++)
    {
        if (!exec_preproc(argv[i]))
        {
            fprintf(stderr, "Failed to process file %s\n", argv[i]);
            continue;
        }
    }

    return 0;
}
