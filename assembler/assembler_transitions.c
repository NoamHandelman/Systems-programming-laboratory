#include "headers/assembler_transitions.h"

/**
 * This is a large enough random number so that we can with a high probability absorb the entire line and make sure that its length is not greater than allowed.
 */

/**
 * @brief Execute the first pass of the assembler.
 * @param input_filename The name of the am file to process.
 * @return 1 if the first pass was successful, 0 otherwise.
 */

int exec_first_pass(const char *input_filename, Macro **macro_list)
{
    FILE *am_file;
    int IC = 0, DC = 0, line_number = 0, should_continue = 1, externs_count = 0;
    char line[INITIAL_BUFFER_SIZE];
    char final_line[MAX_LINE_LENGTH + 1];
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
        if (strlen(line) > MAX_LINE_LENGTH)
        {
            display_error(line, line_number, "Line is too long", input_filename);
            should_continue = 0;
        }

        printf("length : %lu\n", strlen(line));

        strncpy(final_line, line, MAX_LINE_LENGTH);
        final_line[MAX_LINE_LENGTH] = '\0';

        /**
         * Adjust the line so it will be easier to parse.
         */
        handle_spaces(final_line);

        printf("Line %d: %s\n", line_number, final_line);

        if (strstr(final_line, ".data") || strstr(final_line, ".string"))
        {
            handle_data_or_string(final_line, &symbol_table, &DC, data_image, &should_continue, line_number, input_filename, macro_list);

            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries);
                return 0;
            }
        }
        else if (strstr(final_line, ".extern"))
        {
            handle_extern(final_line, &symbol_table, &externs_count, &should_continue, line_number, input_filename, entries, macro_list);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries);
                return 0;
            }
        }
        else if (strstr(final_line, ".entry"))
        {
            handle_entry(final_line, &symbol_table, &entries, &should_continue, line_number, input_filename);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries);
                return 0;
            }
        }
        else
        {
            handle_instruction(final_line, &symbol_table, &IC, code_image, &should_continue, line_number, input_filename, macro_list);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries);
                return 0;
            }
        }
    }

    fclose(am_file);

    return exec_second_pass(input_filename, symbol_table, code_image, data_image, IC, DC, entries, externs_count);
}

int exec_second_pass(const char *input_filename, Symbol *symbol_table, Machine_Code_Image *code_image, Machine_Code_Image *data_image, int IC, int DC, Declaration *entries, int externs_count)
{
    FILE *ent_file, *ext_file;
    char *ob_file_name, *ent_file_name, *ext_file_name;
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

    printf("creating ob file %s\n", ob_file_name);

    convert_to_octal(ob_file_name, code_image, IC, data_image, DC);

    printf("converted to octal\n");

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

        printf("open ent file %s\n", ent_file_name);

        while (current)
        {
            Symbol *symbol = find_symbol(symbol_table, current->name);
            if (!symbol)
            {
                /**
                 * Handle other kind of error like this.
                 */
                printf("Entry symbol not found: %s\n", current->name);
                return 0;
            }
            fprintf(ent_file, "%s 0%d\n", current->name, symbol->address);
            current = current->next;
        }

        fclose(ent_file);
    }

    printf("ent file created\n");
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

/**
 * @brief Free all resources allocated during the assembler execution.
 * @param symbol_table The symbol table to free.
 * @param entries The entries table to free.
 */

void free_all_resources(Symbol *symbol_table, Declaration *entries)
{
    free_symbol_table(symbol_table);
    free_declarations(entries);
}
