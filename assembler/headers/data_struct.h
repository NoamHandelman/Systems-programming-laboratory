#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H
typedef struct Macro
{
    char *name;
    char **content;
    int line_count;
    struct Macro *next;
} Macro;

typedef struct OP_CODE
{
    char *opcode;
    int operands;
} OP_CODE;

typedef struct Symbol
{
    char *name;
    int address;
    int is_external;
    int is_entry;
    struct Symbol *next;
} Symbol;

int create_and_add_symbol(Symbol **, const char *, int, int, int);

Symbol *find_symbol(Symbol *, const char *);

/**
 * remove in the end!!!
 */

void print_symbol_table(Symbol *);

#endif
