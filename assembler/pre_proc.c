#include "headers/pre_proc.h"

/**
 * @brief Function to validate macro definition.
 * @param name The name of the macro.
 * @return 1 if the macro is valid, 0 otherwise.
 */

int validate_macro(const char *name, Macro *macro_list)
{
    if (get_opcode(name) >= 0 || is_valid_instruction(name))
    {
        printf("Macro name can not be a reserved word\n");
        return 0;
    }

    if (find_macro(macro_list, name))
    {
        printf("Macro name already defined\n");
        return 0;
    }

    return 1;
}

/**
 * @brief Handle error in the pre proccess stage.
 * @param message The error message to display.
 * @param line The line that caused the error.
 * @param line_number The line number where the error occurred.
 * @param macro_list The list of macros.
 * @param am_filename The name of the am file.
 * @param as_file The file pointer for the as file.
 * @param am_file The file pointer for the am file.
 * @return NULL (to indicate an error occurred).
 */

void *handle_preproc_error(const char *message, char *line, int line_number, Macro *macro_list, char *am_filename, FILE *as_file, FILE *am_file)
{
    display_error(line, line_number, message, am_filename);
    free_macros(macro_list);
    free(am_filename);
    fclose(as_file);
    fclose(am_file);
    return NULL;
}

/**
 * @brief The main function for the pre proccess stage.
 * @param input_filename The name of the input file.
 * @return the path for the am file.
 */

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

                        display_error(line_copy, line_number, "Extra characters after macro name", input_filename);
                        should_continue = 0;
                    }
                    if (!validate_macro(macro_name, macro_list))
                    {
                        display_error(line_copy, line_number, "Invalid macro name", input_filename);
                        should_continue = 0;
                    }
                    current_macro = create_and_add_macro(&macro_list, macro_name);
                    if (!current_macro)
                    {
                        return handle_preproc_error("Failed to create macro", line, line_number, macro_list, am_filename, as_file, am_file);
                    }
                    in_macro = 1;
                }
                else
                {
                    display_error(line_copy, line_number, "No macro name provided", input_filename);
                    should_continue = 0;
                }
            }
            else if (strcmp(token, "endmacr") == 0)
            {
                char *rest;
                rest = strstr(line_copy, "endmacr") + strlen("endmacr");
                if (check_for_extra_chars(rest))
                {
                    display_error(line_copy, line_number, "Extra characters after end of macro declaration", input_filename);
                    should_continue = 0;
                }
                in_macro = 0;
                current_macro = NULL;
            }
            else if (in_macro)
            {
                if (!add_macro_line(current_macro, line_copy))
                {
                    return handle_preproc_error("Failed to add line to macro", line, line_number, macro_list, am_filename, as_file, am_file);
                };
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
    return should_continue ? am_filename : NULL;
}
