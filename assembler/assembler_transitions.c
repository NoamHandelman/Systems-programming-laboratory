/**
 * This is the file that implements the two passes of the assembler,
 * going over the processed file and analyzing the commands accordingly.
 */

#include "headers/assembler_transitions.h"
#include "./headers/strings.h"
#include "./headers/globals.h"
#include "./headers/lexer.h"
#include "./headers/code_conversions.h"

int exec_first_pass(const char *input_filename, Macro **macro_list)
{
    FILE *am_file;
    int IC = 0, DC = 0, line_number = 0, should_continue = 1, externs_count = 0;

    char final_line[MAX_LINE_LENGTH + 1];

    Symbol *symbol_table = NULL;

    /**
     * Initialize the data and code images.
     */
    Machine_Code_Image_Data data_image[MAX_MEMORY_SIZE] = {0};
    Machine_Code_Image code_image[MAX_MEMORY_SIZE] = {0};
    Declaration *entries = NULL;

    am_file = fopen(input_filename, "r");
    if (!am_file)
    {
        display_system_error("Failed to open file", input_filename);
        return 0;
    }

    /**
     * Read the file line by line while not exceed the allowed memory and process it.
     */

    while (fgets(final_line, sizeof(final_line), am_file) && IC + DC <= MAX_MEMORY_SIZE - MEMORY_START)
    {
        line_number++;

        /**
         * Adjust the line so it will be easier to parse later.
         */
        handle_spaces(final_line);

        /**
         * Handle .data or .string directives, .extern, .entry or instructions.
         * If the proccess failed because of fatal error, we will free all allocated resources and return -1 flag to exit the program.
         * If the proccess failed because of non fatal error, or the proccess was successful we will continue to the second pass (to check more errors or creating the output files).
         */

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
    }

    fclose(am_file);

    /**
     * Check if there is memory overflow, if there is we will display an error and exit the program.
     */

    if (IC + DC > MAX_MEMORY_SIZE - MEMORY_START)
    {
        display_system_error("Memory overflow, exceeded the maximum memory size", input_filename);
        free_all_resources(symbol_table, entries, code_image, IC);
        return -1;
    }

    return exec_second_pass(input_filename, symbol_table, code_image, data_image, IC, DC, entries, externs_count, &should_continue);
}

int exec_second_pass(const char *input_filename, Symbol *symbol_table, Machine_Code_Image *code_image, Machine_Code_Image_Data *data_image, int IC, int DC, Declaration *entries, int externs_count, int *should_continue)
{
    /**
     * Update the addresses of each symbol in the symbol table according the current IC and the symbol definition
     */

    update_symbols_addresses(&symbol_table, IC);

    /**
     * Update the value of each element in the code image which has symbol
     */

    update_symbols_in_code_image(code_image, symbol_table, IC, input_filename, should_continue);

    /**
     * Check the current proccess status, if fatal error found exit the program after free all allocated resources
     */

    if (*should_continue == -1)
    {
        free_all_resources(symbol_table, entries, code_image, IC);
        return -1;
    }

    /**
     * Create object file
     */

    create_ob_file(code_image, IC, data_image, DC, input_filename, should_continue);

    /**
     * If entries table is not empty and create entry file
     */
    if (entries && *should_continue != -1)
    {
        create_ent_file(entries, symbol_table, input_filename, should_continue);
    }

    /**
     * If extern count is not 0 create extern file
     */

    if (externs_count && *should_continue != -1)
    {
        create_ext_file(symbol_table, code_image, IC, input_filename, should_continue);
    }

    /**
     * Free all allocated resources during the first and second passes
     */

    free_all_resources(symbol_table, entries, code_image, IC);

    return *should_continue;
}

/**
 * Free all allocated resources
 */

void free_all_resources(Symbol *symbol_table, Declaration *entries, Machine_Code_Image *code_image, int IC)
{
    free_symbol_table(symbol_table);
    free_declarations(entries);
    free_machine_code_image(code_image, IC);
}
