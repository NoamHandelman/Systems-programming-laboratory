#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/data_struct.h"

#define MEMORY_START 100

/**
 * Macro functions
 */

Macro *create_and_add_macro(Macro **head, const char *name)
{
    Macro *macro = (Macro *)malloc(sizeof(Macro));
    if (!macro)
        return NULL;

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
    char **new_content = (char **)realloc(macro->content, sizeof(char *) * (macro->line_count + 1));
    if (!new_content)
    {
        return 0;
    }

    macro->content = new_content;

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
 * void add_macro_line(Macro *macro, const char *line)
{
    macro->content = (char **)realloc(macro->content, sizeof(char *) * (macro->line_count + 1));
    macro->content[macro->line_count] = (char *)malloc(strlen(line) + 1);
    strcpy(macro->content[macro->line_count], line);
    macro->line_count++;
}
 */

/**
 * void add_macro(Macro **head, Macro *macro)
{
    macro->next = *head;
    *head = macro;
}
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
    new_symbol->is_entry = !is_external;
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

int create_and_add_declaration(Declaration **table, char *name)
{
    Declaration *new_entry = (Declaration *)malloc(sizeof(Declaration));
    printf("Creating decl struct for symbol: %s\n", name);
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
            current = current->next;
        }
        current->next = new_entry;
    }

    return 1;
}

void update_symbols_addresses(Symbol **symbol_table, int IC)
{
    Symbol *current = *symbol_table;
    while (current != NULL)
    {
        if (current->is_data)
        {
            printf("Updating data symbol: %s\n", current->name);
            current->address += (IC + MEMORY_START);
        }
        else if (!current->is_data && !current->is_external)
        {

            current->address += MEMORY_START;
        }
        current = current->next;
    }
}

void update_entry_symbols(Symbol **symbol_table, Declaration **entries)
{
    Declaration *current = *entries;
    while (current != NULL)
    {
        Symbol *symbol = find_symbol(*symbol_table, current->name);
        if (symbol)
        {

            symbol->is_entry = 1;
        }
        else
        {
            fprintf(stderr, "Entry symbol not found: %s\n", current->name);
        }
        current = current->next;
    }
}
