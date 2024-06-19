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

typedef enum
{
    CODE,
    DATA
} LabelType;

typedef enum
{
    A,
    R,
    E
} Are;

typedef struct Label
{
    char *name;
    int value;
    LabelType type;
    Are are;
    struct Label *next;
} Label;
