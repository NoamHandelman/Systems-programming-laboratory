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
    int is_data;
    struct Symbol *next;
} Symbol;

typedef struct Error
{
    char *message;
    int line_number;
} Error;

typedef struct Machine_Code_Image
{
    unsigned short binary;
    int value;
    int is_data;
    int is_string;
    int is_instruction;
    const char *symbol;
} Machine_Code_Image;

int create_and_add_symbol(Symbol **, const char *, int, int, int);

Symbol *find_symbol(Symbol *, const char *);

/**
 * remove in the end!!!
 */

void print_symbol_table(Symbol *);

void update_data_symbols(Symbol **, int);

#endif
