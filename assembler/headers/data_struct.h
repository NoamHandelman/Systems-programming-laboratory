typedef struct Macro
{
    char *name;
    char **content;
    int line_count;
    struct Macro *next;
} Macro;
