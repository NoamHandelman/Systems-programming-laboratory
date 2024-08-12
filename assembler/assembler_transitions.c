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
        printf("IC IS: %d\n", IC);
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
                free_all_resources(symbol_table, entries, code_image, IC);
                return -1;
            }
        }
        else if (strstr(final_line, ".extern"))
        {
            handle_extern(final_line, &symbol_table, &externs_count, &should_continue, line_number, input_filename, entries, macro_list);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries, code_image, IC);
                return -1;
            }
        }
        else if (strstr(final_line, ".entry"))
        {
            handle_entry(final_line, &symbol_table, &entries, &should_continue, line_number, input_filename);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries, code_image, IC);
                return -1;
            }
        }
        else
        {
            handle_instruction(final_line, &symbol_table, &IC, code_image, &should_continue, line_number, input_filename, macro_list);
            if (should_continue == -1)
            {
                free_all_resources(symbol_table, entries, code_image, IC);
                return -1;
            }
        }

        printf("current status: %d\n", should_continue);
    }

    fclose(am_file);

    return exec_second_pass(input_filename, symbol_table, code_image, data_image, IC, DC, entries, externs_count, &should_continue);
}

int exec_second_pass(const char *input_filename, Symbol *symbol_table, Machine_Code_Image *code_image, Machine_Code_Image *data_image, int IC, int DC, Declaration *entries, int externs_count, int *should_continue)
{

    printf("First pass status: %d\n", *should_continue);

    update_symbols_addresses(&symbol_table, IC);

    update_symbols_in_code_image(code_image, symbol_table, IC);

    if (*should_continue == -1)
    {
        free_all_resources(symbol_table, entries, code_image, IC);
        return -1;
    }

    create_ob_file(code_image, IC, data_image, DC, input_filename, should_continue);

    if (entries && *should_continue != -1)
    {
        create_ent_file(entries, symbol_table, input_filename, should_continue);
    }

    if (externs_count && *should_continue != -1)
    {
        create_ext_file(symbol_table, code_image, IC, input_filename, should_continue);
    }

    free_all_resources(symbol_table, entries, code_image, IC);

    printf("Second pass status: %d\n", *should_continue);

    return *should_continue;
}

/**
 * @brief Free all resources allocated during the assembler execution.
 * @param symbol_table The symbol table to free.
 * @param entries The entries table to free.
 */

void free_all_resources(Symbol *symbol_table, Declaration *entries, Machine_Code_Image *code_image, int IC)
{
    free_symbol_table(symbol_table);
    free_declarations(entries);

    free_machine_code_image(code_image, IC);
}
