#include "headers/assembler_transitions.h"

int exec_first_pass(const char *input_filename)
{
    FILE *am_file;
    int IC = 0, DC = 0;
    char line[MAX_LINE_LENGTH];
    Symbol *symbol_table = NULL;
    Data *data_table = NULL;
    /**Code *code_table = NULL;*/

    am_file = fopen(input_filename, "r");
    if (!am_file)
    {
        fprintf(stderr, "Failed to open file: %s\n", input_filename);
        return 0;
    }

    while (fgets(line, sizeof(line), am_file) != NULL && IC + DC <= MAX_MEMORY_SIZE)
    {
        if (is_empty_line(line))
            continue;

        /**remove_spaces(line);*/
        handle_spaces(line);
        printf("%s\n", line);

        if (strstr(line, ".data") || strstr(line, ".string"))
        {
            handle_data_or_string(line, &symbol_table, &DC);
        }
        else if (strstr(line, ".extern") || strstr(line, ".entry"))
        {
            if (strstr(line, ".entry"))
                continue;
            handle_extern(line, &symbol_table);
        }
        else
        {
            handle_instruction(line, &symbol_table, &IC);
        }
    }

    print_symbol_table(symbol_table);

    fclose(am_file);
    return exec_second_pass(input_filename);
}

int exec_second_pass(const char *input_filename)
{
    return 1;
}
