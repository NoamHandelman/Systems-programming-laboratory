#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/data_struct.h"

#define MEMORY_START 100

int create_and_add_symbol(Symbol **symbol_table, const char *name, int address, int is_external, int is_data)
{

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!new_symbol)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    new_symbol->name = (char *)malloc(strlen(name) + 1);
    if (!new_symbol->name)
    {
        fprintf(stderr, "Memory allocation for name failed\n");
        free(new_symbol);
        return 0;
    }
    strcpy(new_symbol->name, name);

    new_symbol->address = address;
    new_symbol->is_entry = 0;
    new_symbol->is_external = is_external;
    new_symbol->is_data = is_data;
    new_symbol->next = NULL;

    if (*symbol_table == NULL)
    {
        *symbol_table = new_symbol;
    }
    else
    {
        Symbol *current = *symbol_table;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_symbol;
    }

    return 1;
}

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

void free_symbol_table(Symbol *symbol_table)
{
    Symbol *current = symbol_table;
    while (current != NULL)
    {
        Symbol *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

/**
 * function to print, remove in the end!!!!
 */

void print_symbol_table(Symbol *symbol_table)
{
    Symbol *current = symbol_table;
    while (current != NULL)
    {
        printf("name: %s, address: %d, is_entry: %d, is_external: %d\n", current->name, current->address, current->is_entry, current->is_external);
        current = current->next;
    }
}

int create_and_add_entry(Declaration **entries, char *name, int *IC)
{
    Declaration *new_entry = (Declaration *)malloc(sizeof(Declaration));
    printf("Creating entry for symbol: %s\n", name);
    if (!new_entry)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    new_entry->name = (char *)malloc(strlen(name) + 1);
    if (!new_entry->name)
    {
        fprintf(stderr, "Memory allocation for name failed\n");
        free(new_entry);
        return 0;
    }

    strcpy(new_entry->name, name);

    new_entry->address = *IC;
    new_entry->next = NULL;

    if (*entries == NULL)
    {
        *entries = new_entry;
    }
    else
    {
        Declaration *current = *entries;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_entry;
    }

    return 1;
}

void update_symbols(Symbol **symbol_table, int IC)
{
    Symbol *current = *symbol_table;
    while (current != NULL)
    {
        if (current->is_data)
        {
            printf("Updating data symbol: %s\n", current->name);
            current->address += (IC + MEMORY_START);
        }
        if (!current->is_data && !current->is_external)
        {

            current->address += MEMORY_START;
        }
        current = current->next;
    }
}
