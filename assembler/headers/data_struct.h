#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

/**
 * Macro struct
 */
typedef struct Macro
{
    char *name;
    char **content;
    int line_count;
    struct Macro *next;
} Macro;

Macro *create_and_add_macro(Macro **, const char *);
/**
 * Macro *create_macro(const char *);

 */

int add_macro_line(Macro *, const char *);

/**
 * void add_macro(Macro **, Macro *);

 */

void free_macros(Macro *);

Macro *find_macro(Macro *, const char *);

/**
 * Symbol struct
 */

typedef struct Symbol
{
    char *name;
    int address;
    int is_external;
    int is_entry;
    int is_data;
    struct Symbol *next;
} Symbol;

int create_and_add_symbol(Symbol **, const char *, int, int, int);

Symbol *find_symbol(Symbol *, const char *);

void free_symbol_table(Symbol *);

/**
 * remove in the end!!!
 */

void print_symbol_table(Symbol *);

void update_symbols_addresses(Symbol **, int);

/**
 * Machine Code Image struct
 */

typedef struct Machine_Code_Image
{
    unsigned short value;
    char *symbol;
} Machine_Code_Image;

void free_machine_code_image(Machine_Code_Image *, int);

/**
 * Instruction struct
 */
typedef struct OP_CODE
{
    char *opcode;
    int operands;
    int src_operands[4];
    int dest_operands[4];
} OP_CODE;

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
    char *op_code;
    Operand operands[2];
    int operand_count;
} Instruction;

/**
 * Declaration struct
 */

typedef struct Declaration
{
    char *name;
    struct Declaration *next;
} Declaration;

int create_and_add_declaration(Declaration **, char *);

void free_declarations(Declaration *);

Declaration *find_declaration(Declaration *, const char *);

#endif
