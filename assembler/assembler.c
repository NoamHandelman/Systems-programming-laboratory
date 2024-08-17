/**
 * @author Noam Handelman
 * @date 18 August 2020
 * The assembler program, the assembler read list of files written in assembly from the command line and create a machine code file from it.
 */

#include "headers/pre_proc.h"
#include "headers/assembler_transitions.h"
#include <stdlib.h>

/**
 * @brief This is the main function of the assembler program, the assembler read list of
 * files written in assembly from the command line and create a machine code file from it.
 * @param argc The number of arguments from the command line (include the executable file name).
 * @param argv The list of the files from the command line.
 */

int main(int argc, char *argv[])
{
    int i;

    /**
     * Paths for the am and as files.
     */

    char *am_file = NULL;
    char *as_file = NULL;

    /**
     * Check if are no files provided by the user.
     */

    if (argc < 2)
    {
        printf("ERROR: No files provided\n");
        return 1;
    }

    /**
     * Iterate over the files and process them.
     */

    for (i = 1; i < argc; i++)
    {
        /**
         * Proccess status flag, 1 for success, 0 for failure, -1 for fatal error.
         */
        int proccess_status = 1;
        Macro *macro_list = NULL;
        as_file = create_file(argv[i], ".as");

        /**
         * Check if the path to .as file created successfully, if not exit the program.
         */
        if (!as_file)
        {
            display_system_error("Failed to create file path", argv[i]);
            return 1;
        }

        /**
         * Execute the pre proccess stage for the current file, if fatal error ocurred exit the program, if error found in the as file continue to the next files.
         */

        am_file = exec_preproc(as_file, &macro_list, &proccess_status);
        if (!am_file)
        {
            free_macros(macro_list);
            free(as_file);
            display_system_error("Failed to process file at pre proccess stage", argv[i]);
            if (proccess_status == -1)
            {
                return 1;
            }
            continue;
        }

        printf("Pre proccess stage done successfully for %s\n", argv[i]);

        /**
         * Execute the assembler stage for the current file, if fatal error ocurred exit the program, if error found in the as file continue to the next files.
         */

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
