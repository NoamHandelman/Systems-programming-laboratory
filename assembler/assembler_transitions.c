#include "headers/assembler_transitions.h"

/**
 * @brief Execute the first pass of the assembler.
 * @param input_filename The name of the am file to process.
 * @return 1 if the first pass was successful, 0 otherwise.
 */

int exec_first_pass(const char *input_filename)
{
    FILE *am_file;
    int IC = 0, DC = 0, line_number = 0, should_continue = 1, externs_count = 0;
    char line[MAX_LINE_LENGTH];
    Symbol *symbol_table = NULL;
    Machine_Code_Image data_image[MAX_MEMORY_SIZE];
    Machine_Code_Image code_image[MAX_MEMORY_SIZE];
    Declaration *entries = NULL;

    am_file = fopen(input_filename, "r");
    if (!am_file)
    {
        printf("ERROR: Failed to open file: %s\n", input_filename);
        return 0;
    }

    /**
     * Read the file line by line while not exceed the allowed memory and process it.
     */

    while (fgets(line, sizeof(line), am_file) && IC + DC <= MAX_MEMORY_SIZE - MEMORY_START)
    {
        line_number++;

        /**
         * Adjust the line so it will be easier to parse.
         */
        handle_spaces(line);

        printf("Line %d: %s\n", line_number, line);

        if (strstr(line, ".data") || strstr(line, ".string"))
        {
            handle_data_or_string(line, &symbol_table, &DC, data_image, &should_continue);
        }
        else if (strstr(line, ".extern"))
        {

            handle_extern(line, &symbol_table, &externs_count);
        }
        else if (strstr(line, ".entry"))
        {
            handle_entry(line, &symbol_table, &entries);
        }
        else
        {
            handle_instruction(line, &symbol_table, &IC, code_image);
        }
    }

    fclose(am_file);

    return exec_second_pass(input_filename, symbol_table, code_image, data_image, IC, DC, entries, externs_count);
}

int exec_second_pass(const char *input_filename, Symbol *symbol_table, Machine_Code_Image *code_image, Machine_Code_Image *data_image, int IC, int DC, Declaration *entries, int externs_count)
{
    FILE *ent_file;
    FILE *ext_file;
    char *ob_file_name;
    char *ent_file_name;
    char *ext_file_name;
    update_entry_symbols(&symbol_table, &entries);
    update_symbols_addresses(&symbol_table, IC);
    update_symbols_in_code_image(code_image, symbol_table);
    print_symbol_table(symbol_table);

    ob_file_name = create_file(input_filename, ".ob");
    if (!ob_file_name)
    {
        fprintf(stderr, "Failed to create file\n");
        return 0;
    }

    convert_to_octal(ob_file_name, code_image, IC, data_image, DC);

    if (entries)
    {
        Declaration *current = entries;
        ent_file_name = create_file(input_filename, ".ent");
        if (!ent_file_name)
        {
            fprintf(stderr, "Failed to create file\n");
            return 0;
        }

        ent_file = fopen(ent_file_name, "w");
        if (!ent_file)
        {
            fprintf(stderr, "Failed to open file %s\n", ent_file_name);
            return 0;
        }

        while (current)
        {
            fprintf(ent_file, "%s 0%d\n", current->name, find_symbol(symbol_table, current->name)->address);
            current = current->next;
        }

        fclose(ent_file);
    }

    printf("Externs count: %d\n", externs_count);

    if (externs_count)
    {
        Symbol *current = symbol_table;
        int i;
        ext_file_name = create_file(input_filename, ".ext");
        if (!ext_file_name)
        {
            fprintf(stderr, "Failed to create file\n");
            return 0;
        }

        ext_file = fopen(ext_file_name, "w");
        if (!ext_file)
        {
            fprintf(stderr, "Failed to open file %s\n", ext_file_name);
            return 0;
        }

        while (current)
        {
            if (current->is_external)
            {
                for (i = 0; i < IC; i++)
                {
                    if (code_image[i].symbol && strcmp(code_image[i].symbol, current->name) == 0)
                    {
                        fprintf(ext_file, "%s 0%d\n", current->name, i + 100);
                    }
                }
            }
            current = current->next;
        }

        fclose(ext_file);
    }

    return 1;
}
