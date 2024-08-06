#include "headers/pre_proc.h"

int validate_macro(const char *name)
{
    if (get_opcode(name) >= 0 || is_valid_instruction(name))
    {
        return 0;
    }
    return 1;
}

void *handle_preproc_error(const char *message, Macro *macro_list, char *am_filename, FILE *as_file, FILE *am_file)
{
    /**
     * display_error
     */

    free_macros(macro_list);
    free(am_filename);
    fclose(as_file);
    fclose(am_file);
    return NULL;
}

char *exec_preproc(const char *input_filename)
{
    FILE *as_file, *am_file = NULL;
    char line[MAX_LINE_LENGTH];
    char *am_filename;
    Macro *macro_list = NULL, *current_macro = NULL;
    int in_macro = 0;
    int should_continue = 1;
    int line_number = 0;

    am_filename = create_file(input_filename, ".am");
    if (!am_filename)
    {
        printf("ERROR: Failed to create file path for %s\n", input_filename);
        return 0;
    }

    as_file = fopen(input_filename, "r");
    if (as_file == NULL)
    {
        printf("ERROR: Failed to open file %s\n", input_filename);
        free(am_filename);
        return 0;
    }

    am_file = fopen(am_filename, "w");
    if (am_file == NULL)
    {
        printf("ERROR: Failed to open file %s\n", input_filename);
        free(am_filename);
        fclose(as_file);
        return 0;
    }

    while (fgets(line, sizeof(line), as_file))
    {
        char *token, *macro_name;
        char line_copy[MAX_LINE_LENGTH];
        line_number++;
        strcpy(line_copy, line);
        if (is_empty_line(line))
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
                    if (check_for_extra_chars(rest))
                    {
                        /**
                         *  return handle_preproc_error("Extra characters after macro name", macro_list, am_filename, as_file, am_file);
                         */
                        display_error(line, line_number, "Extra characters after macro name", input_filename);
                        should_continue = 0;
                    }
                    if (!validate_macro(macro_name))
                    {
                        /**
                         * return handle_preproc_error("Invalid macro name", macro_list, am_filename, as_file, am_file);
                         */
                        display_error(line, line_number, "Invalid macro name", input_filename);
                        should_continue = 0;
                    }
                    current_macro = create_and_add_macro(&macro_list, macro_name);
                    if (!current_macro)
                    {
                        return handle_preproc_error("Failed to create macro", macro_list, am_filename, as_file, am_file);
                    }
                    /**
                     * add_macro(&macro_list, current_macro);
                     */
                    in_macro = 1;
                }
                else
                {
                    /**
                     * return handle_preproc_error("No macro name provided", macro_list, am_filename, as_file, am_file);
                     */
                    display_error(line, line_number, "No macro name provided", input_filename);
                    should_continue = 0;
                }
            }
            else if (strcmp(token, "endmacr") == 0)
            {
                char *rest;
                rest = strstr(line_copy, "endmacr") + strlen("endmacr");
                if (check_for_extra_chars(rest))
                {
                    return handle_preproc_error("Extra characters after endmacr", macro_list, am_filename, as_file, am_file);
                }
                in_macro = 0;
                current_macro = NULL;
            }
            else if (in_macro)
            {
                add_macro_line(current_macro, line_copy);
            }
            else
            {
                Macro *macro;
                macro = find_macro(macro_list, token);
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
    free_macros(macro_list);
    return am_filename;
}
