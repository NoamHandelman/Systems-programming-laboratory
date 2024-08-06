/**
 * @brief This is the main function of the assembler program, the assembler read list of
 * files written in assembly from the command line and create a machine code file from it.
 * @param argc The number of arguments from the command line (include the executable file name).
 * @param argv The list of the files from the command line.
 */

#include "headers/pre_proc.h"
#include "headers/files.h"
#include "headers/assembler_transitions.h"
#include "headers/errors.h"

int main(int argc, char *argv[])
{
    int i;

    char *am_file;
    char *as_file;

    if (argc < 2)
    {
        printf("ERROR: No files detected\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        as_file = create_file(argv[i], ".as");
        if (!as_file)
        {
            printf("ERROR: Failed to create file path for %s\n", argv[i]);
            free(as_file);
            continue;
        }

        am_file = exec_preproc(as_file);
        if (!am_file)
        {
            printf("ERROR: Failed to process %s at pre proccess stage\n", argv[i]);
            continue;
        }

        if (!(exec_first_pass(am_file)))
        {
            printf("ERROR: Failed to process %s at assembler stage\n", argv[i]);
        };

        free(as_file);
        free(am_file);
    }

    printf("Done process all files\n");
    return 0;
}
