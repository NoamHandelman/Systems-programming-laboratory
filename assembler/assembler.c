#include "headers/pre_proc.h"
#include "headers/memory.h"

int main(int argc, char *argv[])
{
    int i;
    char *am_file;

    if (argc < 2)
    {
        fprintf(stderr, "No input files\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        am_file = exec_preproc(argv[i]);
        if (!am_file)
        {
            fprintf(stderr, "Failed to process file at pre proccess stage %s\n", argv[i]);
            continue;
        }

        if (!(exec_assembler_transitions(am_file)))
        {
            fprintf(stderr, "Failed to process file at assembler stage %s\n", argv[i]);
            continue;
        };

        free(am_file);
    }

    printf("Done assembler process\n");
    return 0;
}
