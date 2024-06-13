#include "headers/globals.h"
#include "headers/pre_proc.h"
#include "headers/memory_helper.h"

Macro *create_macro(const char *name)
{
    Macro *macro = (Macro *)allocate_memory(sizeof(Macro));
    if (!macro)
        return NULL;

    macro->name = (char *)allocate_memory(strlen(name) + 1);
    if (!macro->name)
    {
        free(macro);
        return NULL;
    }
    strcpy(macro->name, name);

    macro->content = NULL;
    macro->line_count = 0;
    macro->next = NULL;

    return macro;
}

void add_macro_line(Macro *macro, const char *line)
{
    macro->content = (char **)realloc(macro->content, sizeof(char *) * (macro->line_count + 1));
    macro->content[macro->line_count] = (char *)allocate_memory(strlen(line) + 1);
    strcpy(macro->content[macro->line_count], line);
    macro->line_count++;
}

void add_macro(Macro **head, Macro *macro)
{
    macro->next = *head;
    *head = macro;
}

void free_macros(Macro *head)
{
    int i;
    Macro *current = head;
    while (current)
    {
        Macro *next = current->next;
        for (i = 0; i < current->line_count; i++)
        {
            free(current->content[i]);
        }
        free(current->content);
        free(current->name);
        free(current);
        current = next;
    }
}

Macro *find_macro(Macro *head, const char *name)
{
    while (head)
    {
        if (strcmp(head->name, name) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

int validate_macro(const char *name)
{
    /**
     * Check if the macro name is valid
     */
    return 1;
}

void *handle_preproc_error(const char *message, Macro *macro_list, char *am_filename, FILE *as_file, FILE *am_file)
{
    fprintf(stderr, "%s\n", message);
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

    size_t am_filename_length = strlen(input_filename) + 4;
    am_filename = (char *)allocate_memory(am_filename_length);
    if (!am_filename)
    {
        fprintf(stderr, "Failed to allocate memory for preprocess file\n");
        return 0;
    }

    snprintf(am_filename, am_filename_length, "%s.am", input_filename);

    as_file = fopen(input_filename, "r");
    if (as_file == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", input_filename);
        free(am_filename);
        return 0;
    }

    am_file = fopen(am_filename, "w");
    if (am_file == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", am_filename);
        free(am_filename);
        fclose(as_file);
        return 0;
    }

    while (fgets(line, sizeof(line), as_file) != NULL)
    {
        char *token, *macro_name;
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);
        token = strtok(line, " \t\n");
        if (token)
        {
            if (strcmp(token, "macr") == 0)
            {
                macro_name = strtok(NULL, " \t\n");
                if (macro_name)
                {
                    if (!validate_macro(macro_name))
                    {
                        return handle_preproc_error("Invalid macro name", macro_list, am_filename, as_file, am_file);
                    }
                    current_macro = create_macro(macro_name);
                    if (!current_macro)
                    {
                        return handle_preproc_error("Failed to create macro", macro_list, am_filename, as_file, am_file);
                    }
                    add_macro(&macro_list, current_macro);
                    in_macro = 1;
                }
                else
                {
                    return handle_preproc_error("No macro name provided", macro_list, am_filename, as_file, am_file);
                }
            }
            else if (strcmp(token, "endmacr") == 0)
            {
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
