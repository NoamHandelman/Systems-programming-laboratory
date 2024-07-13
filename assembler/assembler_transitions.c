#include "headers/assembler_transitions.h"

int exec_first_pass(const char *input_filename)
{
    FILE *am_file;
    int IC = 0, DC = 0, line_number = 0;
    char line[MAX_LINE_LENGTH];
    Symbol *symbol_table = NULL;
    Machine_Code_Image data_image[2048];
    Machine_Code_Image code_image[2048];

    am_file = fopen(input_filename, "r");
    if (!am_file)
    {
        fprintf(stderr, "Failed to open file: %s\n", input_filename);
        return 0;
    }

    while (fgets(line, sizeof(line), am_file) && IC + DC <= MAX_MEMORY_SIZE)
    {
        line_number++;
        if (is_empty_line(line))
            continue;

        handle_spaces(line);

        printf("Line %d: %s\n", line_number, line);

        if (strstr(line, ".data") || strstr(line, ".string"))
        {
            handle_data_or_string(line, &symbol_table, &DC, data_image);
        }
        else if (strstr(line, ".extern"))
        {
            handle_extern(line, &symbol_table);
        }
        else
        {
            handle_instruction(line, &symbol_table, &IC);
        }
    }

    update_data_symbols(&symbol_table, IC);
    print_symbol_table(symbol_table);

    fclose(am_file);
    return exec_second_pass(input_filename);
}

int exec_second_pass(const char *input_filename)
{
    return 1;
}
