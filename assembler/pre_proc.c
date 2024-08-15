/**
 * This file contains the implementation of the pre proccess stage.
 */

#include <string.h>
#include <stdlib.h>

#include "./headers/pre_proc.h"
#include "./headers/globals.h"
#include "./headers/strings.h"
#include "./headers/lexer.h"
#include "./headers/files.h"
#include "./headers/errors.h"

/**
 * @brief Function to validate macro definition.
 * @param name The name of the macro.
 * @param macro_list The list of macros.
 * @param line The line that caused the error.
 * @param line_number The line number where the error occurred.
 * @param am_filename The name of the am file.
 * @return 1 if the macro is valid, 0 otherwise (If the name length is bigger then 31, or if the name is a reserved word, or if its already defined).
 */

int validate_macro(const char *name, Macro *macro_list, char *line, int line_number, const char *am_filename);

/**
 * @brief Handle error in the pre proccess stage.
 * @param message The error message to display.
 * @param line The line that caused the error.
 * @param line_number The line number where the error occurred.
 * @param am_filename The name of the am file.
 * @param as_file The file pointer for the as file.
 * @param am_file The file pointer for the am file.
 * @param current_macro The current macro that was created.
 */

void handle_preproc_error(const char *message, char *line, int line_number, char *am_filename, FILE *as_file, FILE *am_file);

int validate_macro(const char *name, Macro *macro_list, char *line, int line_number, const char *am_filename)
{
    /**
     * Check if the macro name is too long.
     */
    if (strlen(name) > MAX_SYMBOL_LENGTH)
    {
        display_error(line, line_number, "Macro name is too long", am_filename);
        return 0;
    }

    /**
     * Check if the macro name is a reserved word.
     */

    if (get_opcode(name) >= 0 || is_valid_instruction(name) || get_register(name) >= 0 || strcmp(name, "macr") == 0 || strcmp(name, "endmacr") == 0)
    {
        display_error(line, line_number, "Macro name can not be a reserved word", am_filename);
        return 0;
    }

    /**
     * Check if the macro is already defined.
     */

    if (find_macro(macro_list, name))
    {
        display_error(line, line_number, "Macro name already defined", am_filename);
        return 0;
    }

    return 1;
}

void handle_preproc_error(const char *message, char *line, int line_number, char *am_filename, FILE *as_file, FILE *am_file)
{
    display_error(line, line_number, message, am_filename);
    free(am_filename);
    fclose(as_file);
    fclose(am_file);
    remove(am_filename);
}

char *exec_preproc(const char *input_filename, Macro **macro_list, int *proccess_status)
{
    FILE *as_file = NULL, *am_file = NULL;
    /**
     * In the pre process stage we will hold the line on a very large buffer and only on the firs pass we will valid the line length.
     */
    char line[INITIAL_BUFFER_SIZE];
    char *am_filename = NULL;
    Macro *current_macro = NULL;
    int in_macro = 0, line_number = 0;

    am_filename = create_file(input_filename, ".am");
    if (!am_filename)
    {
        display_system_error("Failed to create file path", input_filename);
        *proccess_status = -1;
        return NULL;
    }

    as_file = fopen(input_filename, "r");
    if (!as_file)
    {
        display_system_error("Failed to open file", input_filename);
        free(am_filename);
        *proccess_status = 0;
        return NULL;
    }

    am_file = fopen(am_filename, "w");
    if (!am_file)
    {
        display_system_error("Failed to open file", am_filename);
        free(am_filename);
        fclose(as_file);
        *proccess_status = 0;
        return NULL;
    }

    while (fgets(line, sizeof(line), as_file))
    {
        char *token = NULL, *macro_name = NULL;
        char line_copy[INITIAL_BUFFER_SIZE];
        line_number++;
        strcpy(line_copy, line);

        /**
         * Skip empty lines and comments lines.
         */

        if (is_empty_line(line) || line[0] == ';')
        {
            continue;
        }

        token = strtok(line, " \t\n");
        if (token)
        {
            if (strcmp(token, "macr") == 0)
            {
                macro_name = strtok(NULL, " \t\n");
                if (macro_name)
                {
                    char *rest;
                    rest = strstr(line_copy, macro_name) + strlen(macro_name);
                    /**
                     * Check for extra characters after the macro name.
                     */
                    if (check_for_extra_chars(rest))
                    {
                        display_error(line_copy, line_number, "Extra characters after macro name", input_filename);
                        *proccess_status = 0;
                    }

                    /**
                     * Validate the macro name.
                     */
                    if (!validate_macro(macro_name, *macro_list, line_copy, line_number, input_filename))
                    {
                        *proccess_status = 0;
                    }
                    else
                    {
                        current_macro = create_and_add_macro(macro_list, macro_name);

                        /**
                         * Check if the macro was created successfully, if not exit the program.
                         */

                        if (!current_macro)
                        {
                            handle_preproc_error("Failed to create macro", line, line_number, am_filename, as_file, am_file);
                            *proccess_status = -1;
                            return NULL;
                        }
                        in_macro = 1;
                    }
                }
                else
                {
                    display_error(line_copy, line_number, "No macro name provided", input_filename);
                    *proccess_status = 0;
                }
            }
            else if (strcmp(token, "endmacr") == 0)
            {
                /**
                 * Check for extra characters after end of macro declaration.
                 */
                char *rest;
                rest = strstr(line_copy, "endmacr") + strlen("endmacr");
                if (check_for_extra_chars(rest))
                {
                    display_error(line_copy, line_number, "Extra characters after end of macro declaration", input_filename);
                    *proccess_status = 0;
                }
                in_macro = 0;
                current_macro = NULL;
            }
            else if (in_macro)
            {
                /**
                 * Add the line to the current macro, if the allocation failed exit the program.
                 */
                if (!add_macro_line(current_macro, line_copy))
                {
                    handle_preproc_error("Failed to add line to macro", line, line_number, am_filename, as_file, am_file);
                    *proccess_status = -1;
                    return NULL;
                };
            }
            else
            {
                Macro *macro;
                macro = find_macro(*macro_list, token);
                if (macro)
                {
                    int i;
                    for (i = 0; i < macro->line_count; i++)
                    {
                        fprintf(am_file, "%s", macro->content[i]);
                    }
                }
                else
                {
                    fprintf(am_file, "%s", line_copy);
                }
            }
        }
        else if (!in_macro)
        {
            fprintf(am_file, "%s", line_copy);
        }
    }

    fclose(as_file);
    fclose(am_file);

    /**
     * Check if the proccess was successful, if not remove the am file and free the allocated resources.
     */

    if (!(*proccess_status))
    {
        remove(am_filename);
        free(am_filename);
        return NULL;
    }

    return am_filename;
}
