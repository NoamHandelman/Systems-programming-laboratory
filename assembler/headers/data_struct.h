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

typedef struct Machine_Code_Image
{
    unsigned short value;
    const char *symbol;
} Machine_Code_Image;

typedef struct Operand
{
    char *op_type;
    union
    {
        int reg;
        int num;
        char *symbol;
    } value;

    int addressing_mode;

} Operand;

typedef struct Instruction
{
    OP_CODE op_code;
    Operand operands[2];
    int operand_count;
} Instruction;

typedef struct Declaration
{
    char *name;
    int address;
    struct Declaration *next;
} Declaration;

int create_and_add_symbol(Symbol **, const char *, int, int, int);

Symbol *find_symbol(Symbol *, const char *);

/**
 * remove in the end!!!
 */

void print_symbol_table(Symbol *);

void update_symbols_addresses(Symbol **, int);

int create_and_add_declaration(Declaration **, char *, int);

Declaration *find_declaration(Symbol *, const char *);

void update_entry_symbols(Symbol **, Declaration **);

void update_extern_symbols(Symbol **, Declaration **);

#endif
