/**
 * This file contains the the data structures and their related functions that used in the assembler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/data_struct.h"
#include "../headers/globals.h"

/**
 * Macro functions
 */

Macro *create_and_add_macro(Macro **head, const char *name)
{
    Macro *macro = (Macro *)malloc(sizeof(Macro));
    if (!macro)
        return NULL;

    /**
     * Allocate memory for the macro name and try to copy the name to the new allocated memory and insert the macro to the list,
     * while initialize its fields.
     */

    macro->name = (char *)malloc(strlen(name) + 1);
    if (!macro->name)
    {
        free(macro);
        return NULL;
    }
    strcpy(macro->name, name);

    macro->content = NULL;
    macro->line_count = 0;
    macro->next = *head;
    *head = macro;

    return macro;
}

int add_macro_line(Macro *macro, const char *line)
{
    /**
     * Allocate memory for the new line and try to copy the line to the new allocated memory and insert the line to the macro content.
     */
    char **new_line = (char **)realloc(macro->content, sizeof(char *) * (macro->line_count + 1));
    if (!new_line)
    {
        return 0;
    }

    macro->content = new_line;

    macro->content[macro->line_count] = (char *)malloc(strlen(line) + 1);
    if (!macro->content[macro->line_count])
    {
        return 0;
    }

    strcpy(macro->content[macro->line_count], line);
    macro->line_count++;

    return 1;
}

/**
 * Free the memory allocated for the macros list.
 */

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

/**
 * Find a macro in the macros list by the macro name.
 */

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

/**
 * Symbol functions
 */

int create_and_add_symbol(Symbol **symbol_table, const char *name, int address, int is_external, int is_data)
{

    /**
     * Allocate memory for the new symbol and try to copy the name to the new allocated memory and insert the symbol to the symbol table,
     */
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!new_symbol)
    {
        return 0;
    }

    new_symbol->name = (char *)malloc(strlen(name) + 1);
    if (!new_symbol->name)
    {
        free(new_symbol);
        return 0;
    }
    strcpy(new_symbol->name, name);

    new_symbol->address = address;
    /**
     * If symbol is not extern, initialize the is_entry field to 1, otherwise initialize the is_external field to 1.
     */
    new_symbol->is_entry = !is_external;
    new_symbol->is_external = is_external;
    /**
     * Flag to indicate if the symbol is declared in .data or .string line or in instruction line.
     */
    new_symbol->is_data = is_data;
    new_symbol->next = NULL;

    /**
     * If the symbol table is empty, insert the new symbol to the symbol table, otherwise iterate over the symbol table and insert the new symbol to the end of the list.
     */

    if (*symbol_table == NULL)
    {
        *symbol_table = new_symbol;
    }
    else
    {
        Symbol *current = *symbol_table;
        while (current->next != NULL)
        {
            if (strcmp(current->name, name) == 0 && current->is_external && is_external)
            {
                /**
                 * If the symbol is already declared as external, ignore the insertion of the new symbol.
                 */
                printf("Symbol already declared as is_external\n");
                free(new_symbol->name);
                free(new_symbol);
                return 1;
            }
            current = current->next;
        }
        if (strcmp(current->name, name) == 0 && current->is_external && is_external)
        {
            /**
             * If the current last symbol is already declared as external, ignore the insertion of the new symbol.
             */
            printf("Symbol already declared as is_external\n");
            free(new_symbol->name);
            free(new_symbol);
            return 1;
        }
        else
        {
            current->next = new_symbol;
        }
    }

    return 1;
}

/**
 * Find a symbol in the symbol table by the symbol name.
 */

Symbol *find_symbol(Symbol *symbol_table, const char *name)
{
    Symbol *current = symbol_table;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Free the memory allocated for the symbol table.
 */

void free_symbol_table(Symbol *symbol_table)
{
    Symbol *current = symbol_table;
    while (current)
    {
        Symbol *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

/**
 * Update the addresses of the symbols in the symbol table after the first pass.
 */

void update_symbols_addresses(Symbol **symbol_table, int IC)
{
    Symbol *current = *symbol_table;
    while (current)
    {
        if (current->is_data)
        {
            /**
             * Make sure that the data part of the memory starts after the code part.
             */
            current->address += (IC + MEMORY_START);
        }
        else if (!current->is_data && !current->is_external)
        {
            current->address += MEMORY_START;
        }
        current = current->next;
    }
}

/**
 * Declaration functions
 */

int create_and_add_declaration(Declaration **table, char *name, int line_number)
{
    Declaration *new_entry = (Declaration *)malloc(sizeof(Declaration));
    if (!new_entry)
    {
        return 0;
    }

    new_entry->name = (char *)malloc(strlen(name) + 1);
    if (!new_entry->name)
    {
        free(new_entry);
        return 0;
    }

    strcpy(new_entry->name, name);

    new_entry->line_number = line_number;
    new_entry->next = NULL;

    if (*table == NULL)
    {
        *table = new_entry;
    }
    else
    {
        Declaration *current = *table;
        while (current->next != NULL)
        {
            if (strcmp(current->name, name) == 0)
            {
                /**
                 * If the symbol is already declared as entry, ignore the insertion of the new symbol.
                 */
                printf("Symbol already declared as entry\n");
                free(new_entry->name);
                free(new_entry);
                return 1;
            }
            current = current->next;
        }
        if (strcmp(current->name, name) == 0)
        {
            /**
             * If the current last symbol is already declared as entry, ignore the insertion of the new symbol.
             */
            printf("Symbol already declared as entry\n");
            free(new_entry->name);
            free(new_entry);
            return 1;
        }
        else
        {
            current->next = new_entry;
        }
    }

    return 1;
}

/**
 * Free the memory allocated for the declaration table.
 */

void free_declarations(Declaration *table)
{
    Declaration *current = table;
    while (current)
    {
        Declaration *next = current->next;
        free(current->name);
        current->name = NULL;
        current->line_number = 0;
        free(current);
        current = next;
    }
}

/**
 * Find a declaration in the declaration table by the declaration name.
 */

Declaration *find_declaration(Declaration *table, const char *name)
{
    Declaration *current = table;
    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Machine Code Image functions
 */

/**
 * Free the memory allocated for the machine code image.
 */

void free_machine_code_image(Machine_Code_Image *code_image, int IC)
{
    int i;
    for (i = 0; i < IC; i++)
    {
        code_image[i].value = 0;
        free(code_image[i].symbol);
        code_image[i].symbol = NULL;
        code_image[i].line_number = 0;
    }
}

/**
 * Instruction functions
 */

/**
 * Free the memory allocated for the instruction struct and its associated symbols if exists.
 */
void free_instruction(Instruction *instruction)
{
    int i;
    if (instruction)
    {
        if (instruction->op_code)
        {
            free(instruction->op_code);
        }
        for (i = 0; i < instruction->operand_count; i++)
        {
            if (instruction->operands[i].addressing_mode == DIRECT && instruction->operands[i].value.symbol)
            {
                free(instruction->operands[i].value.symbol);
            }
        }
        free(instruction);
    }
}
