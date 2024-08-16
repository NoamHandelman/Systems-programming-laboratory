/**
 * The file which contains the implementation of the functions that are responsible for the files handling.
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/files.h"
#include "../headers/globals.h"
#include "../headers/errors.h"

#define FIFTEEN_BIT_MASK 0x7FFF

/**
 * Create a file name with the given filename and extension.
 */

char *create_file(const char *filename, const char *extension)
{
    char *file_path = NULL;
    char *end_pos = strrchr(filename, '.');

    size_t filename_length = end_pos ? (end_pos - filename) : strlen(filename);

    file_path = (char *)malloc(filename_length + strlen(extension) + 1);

    if (!file_path)
    {
        return NULL;
    }

    strncpy(file_path, filename, filename_length);
    file_path[filename_length] = '\0';
    strcat(file_path, extension);

    return file_path;
}

/**
 * Create the object file.
 */

void create_ob_file(Machine_Code_Image *code_image, int IC, Machine_Code_Image_Data *data_image, int DC, const char *input_filename, int *should_continue)
{
    int i;
    /**
     * The address of the first word in the memory that start in 100.
     */
    int address = MEMORY_START;
    FILE *output_file;
    char *ob_file_name = NULL;
    ob_file_name = create_file(input_filename, ".ob");
    if (!ob_file_name)
    {
        display_system_error("Failed to create file path", input_filename);
        *should_continue = -1;
        return;
    }

    output_file = fopen(ob_file_name, "w");
    if (!output_file)
    {
        display_system_error("Failed to open file", ob_file_name);
        free(ob_file_name);
        *should_continue = -1;
        return;
    }

    fprintf(output_file, "%5d %d\n", IC, DC);

    /**
     * Iterate over the code image and data image and write the values to the object file in octal base.
     */

    for (i = 0; i < IC; i++)
    {
        fprintf(output_file, "%04d ", address++);
        fprintf(output_file, "%05o\n", code_image[i].value & FIFTEEN_BIT_MASK);
    }

    for (i = 0; i < DC; i++)
    {
        fprintf(output_file, "%04d ", address++);
        fprintf(output_file, "%05o\n", data_image[i].value & FIFTEEN_BIT_MASK);
    }

    fclose(output_file);
    if (!(*should_continue))
    {
        remove(ob_file_name);
    }
    free(ob_file_name);
}

/**
 * Create the entries file.
 */

void create_ent_file(Declaration *entries, Symbol *symbol_table, const char *input_filename, int *should_continue)
{
    Declaration *current = entries;
    FILE *ent_file;
    char *ent_file_name = NULL;
    ent_file_name = create_file(input_filename, ".ent");
    if (!ent_file_name)
    {
        display_system_error("Failed to create file path", input_filename);
        *should_continue = -1;
        return;
    }

    ent_file = fopen(ent_file_name, "w");
    if (!ent_file)
    {
        display_system_error("Failed to open file", ent_file_name);
        free(ent_file_name);
        *should_continue = -1;
        return;
    }

    while (current)
    {
        Symbol *symbol = find_symbol(symbol_table, current->name);
        if (!symbol)
        {
            /**
             * If symbol that declared as entry not found in the symbol table, display an error and set the should_continue flag to 0.
             */
            display_system_error("Failed to find symbol", input_filename);
            *should_continue = 0;
        }
        else
        {
            /**
             * Write the symbol name and its address to the entries file.
             */
            fprintf(ent_file, "%-31s %04d\n", current->name, symbol->address);
        }
        current = current->next;
    }

    fclose(ent_file);
    if (!(*should_continue))
    {
        remove(ent_file_name);
    }

    free(ent_file_name);
}

/**
 * Create the extern file.
 */

void create_ext_file(Symbol *symbol_table, Machine_Code_Image *code_image, int IC, const char *input_filename, int *should_continue)
{
    Symbol *current = symbol_table;
    FILE *ext_file;
    char *ext_file_name = NULL;
    int i;
    ext_file_name = create_file(input_filename, ".ext");
    if (!ext_file_name)
    {
        display_system_error("Failed to create file path", input_filename);
        *should_continue = -1;
        return;
    }

    ext_file = fopen(ext_file_name, "w");
    if (!ext_file)
    {
        display_system_error("Failed to open file", ext_file_name);
        free(ext_file_name);
        *should_continue = -1;
        return;
    }

    /**
     * Iterate over the symbol table, if the current symbol is extern check for all its occurrences in the code image and write those addresses to the extern file.
     */

    while (current)
    {
        if (current->is_external)
        {
            for (i = 0; i < IC; i++)
            {
                if (code_image[i].symbol && strcmp(code_image[i].symbol, current->name) == 0)
                {
                    fprintf(ext_file, "%-31s %04d\n", current->name, i + MEMORY_START);
                }
            }
        }
        current = current->next;
    }

    fclose(ext_file);
    if (!(*should_continue))
    {
        remove(ext_file_name);
    }
    free(ext_file_name);
}
