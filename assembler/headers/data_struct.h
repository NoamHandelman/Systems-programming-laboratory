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

typedef enum SymbolType
{
    ABSOLUTE,
    EXTERNAL,
    RELOCATABLE
} SymbolType;

typedef enum DirectiveType
{
    DATA,
    STRING,
    ENTRY,
    EXTERN
} DirectiveType;

typedef struct Symbol
{
    char *name;
    int address;
    SymbolType symbol_type;
    DirectiveType directive_type;
    struct SYMBOL *next;
} Symbol;
