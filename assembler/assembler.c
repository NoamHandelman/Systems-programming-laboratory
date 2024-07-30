/**
 * @brief This is the main function of the assembler program, the assembler read list of
 * files written in assembly from the command line and create a machine code file from it.
 * @param argc The number of arguments from the command line (include the executable file name).
 * @param argv The list of the files from the command line.
 */

#include "headers/pre_proc.h"
#include "headers/files.h"
#include "headers/assembler_transitions.h"

int main(int argc, char *argv[])
{
    int i;
    char *am_file;
    char *as_file;

    if (argc < 2)
    {
        fprintf(stderr, "No input files\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        as_file = create_file(argv[i], ".as");
        if (!as_file)
        {
            fprintf(stderr, "Failed to create file path for %s\n", argv[i]);
            continue;
        }

        am_file = exec_preproc(as_file);
        if (!am_file)
        {
            fprintf(stderr, "Failed to process file at pre proccess stage %s\n", argv[i]);
            continue;
        }

        if (!(exec_first_pass(am_file)))
        {
            fprintf(stderr, "Failed to process file at assembler stage %s\n", argv[i]);
            continue;
        };

        free(am_file);
    }

    printf("Done assembler process\n");
    return 0;
}
