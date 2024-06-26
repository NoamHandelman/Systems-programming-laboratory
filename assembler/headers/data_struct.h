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
    struct SYMBOL *next;
} Symbol;

typedef struct Variable
{
    char *name;
    int address;
    struct Variable *next;
} Variable;

int create_and_add_symbol(Symbol **, const char *, int, int, int);

Symbol *find_symbol(Symbol *, const char *);
