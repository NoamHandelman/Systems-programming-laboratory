/**
 * @brief This is the main function of the assembler program, the assembler read list of
 * files written in assembly from the command line and create a machine code file from it.
 * @param argc The number of arguments from the command line (include the executable file name).
 * @param argv The list of the files from the command line.
 */

#include "headers/pre_proc.h"
#include "headers/assembler_transitions.h"

int main(int argc, char *argv[])
{
    int i;

    char *am_file = NULL;
    char *as_file = NULL;

    if (argc < 2)
    {
        printf("ERROR: No files detected\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        int proccess_status = 1;
        Macro *macro_list = NULL;
        as_file = create_file(argv[i], ".as");
        if (!as_file)
        {
            display_system_error("Failed to create file path", argv[i]);
            free(as_file);
            return 1;
        }

        am_file = exec_preproc(as_file, &macro_list, &proccess_status);
        if (!am_file)
        {
            free_macros(macro_list);
            display_system_error("Failed to process file at pre proccess stage", argv[i]);
            if (proccess_status == -1)
            {
                return 1;
            }
            continue;
        }

        printf("Pre proccess stage done successfully for %s\n", argv[i]);

        proccess_status = exec_first_pass(am_file, &macro_list);
        free(as_file);
        free(am_file);
        free_macros(macro_list);

        if (proccess_status <= 0)
        {
            display_system_error("Failed to process file at assembler stage", argv[i]);
            if (proccess_status == -1)
            {
                return 1;
            }
        }
        else
        {
            printf("Assembler stage done successfully for %s\n", argv[i]);
        }
    }

    printf("Done process all files\n");
    return 0;
}
