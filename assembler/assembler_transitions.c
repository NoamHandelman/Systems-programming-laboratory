#include "headers/assembler_transitions.h"

int exec_first_pass(const char *input_filename)
{
    FILE *am_file;
    int IC = 0, DC = 0, line_number = 0, should_continue = 1;
    char line[MAX_LINE_LENGTH];
    Symbol *symbol_table = NULL;
    Machine_Code_Image data_image[2048];
    Machine_Code_Image code_image[2048];
    Declaration *entries = NULL;
    /**
     *     Declaration *externs = NULL;

     */

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
        else if (strstr(line, ".entry"))
        {
            handle_entry(line, &symbol_table, IC, &entries);
        }
        else
        {
            handle_instruction(line, &symbol_table, &IC, code_image);
        }
    }

    fclose(am_file);

    if (should_continue)
    {
        int t;
        int k;
        update_entry_symbols(&symbol_table, &entries);
        update_symbols_addresses(&symbol_table, IC);
        update_symbols_in_code_image(code_image, symbol_table);
        print_symbol_table(symbol_table);
        printf("DC is : %d\n", DC);
        printf("IC is : %d\n", IC);
        for (t = 0; t < IC; t++)
        {
            for (k = 0; k < 15; k++)
            {
                printf("%d", (code_image[t].value >> (15 - 1 - k)) & 1);
            }
            printf("\n");
        }
        printf("Data image:\n");
        for (t = 0; t < DC; t++)
        {
            for (k = 0; k < 15; k++)
            {
                printf("%d", (data_image[t].value >> (15 - 1 - k)) & 1);
            }
            printf("\n");
        }
        return exec_second_pass(input_filename);
    }

    printf("Failed to process file in first pass : %s\n", input_filename);
    return 0;
}

int exec_second_pass(const char *input_filename)
{
    return 1;
}
