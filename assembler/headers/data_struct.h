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


typedef struct SYMBOL
{
    char *name;
    int address;
    int is_external;
    int is_relocatable;
    int is_data;
    int is_code;
    struct SYMBOL *next;
} SYMBOL;
