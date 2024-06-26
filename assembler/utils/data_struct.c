#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/data_struct.h"

int create_and_add_symbol(Symbol **symbol_table, const char *name, int address, int is_entry, int is_external)
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
    new_symbol->is_entry = is_entry;
    new_symbol->is_external = is_external;
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
