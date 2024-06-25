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
    int is_code;
    struct SYMBOL *next;
} Symbol;

// typedef struct Line
// {
//     char *label;
//     char *opcode;
//     char *operand1;
//     char *operand2;
//     int line_number;
// } Line;

typedef struct Variable
{
    char *name;
    int address;
    struct Variable *next;
} Variable;
