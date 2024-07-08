#include <string.h>
#include "../headers/globals.h"
#include "../headers/data_struct.h"
#include "../headers/lexer.h"

OP_CODE OP_CODES[] = {
    {"mov", 2},
    {"cmp", 2},
    {"add", 2},
    {"sub", 2},
    {"lea", 1},
    {"clr", 1},
    {"not", 2},
    {"inc", 1},
    {"dec", 1},
    {"jmp", 1},
    {"bne", 1},
    {"red", 1},
    {"prn", 1},
    {"jsr", 1},
    {"rts", 0},
    {"stop", 0}};

/* Define the registers */
char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Define the instructions */
char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};

int get_opcode(const char *op)
{
    int i;
    for (i = 0; i < OP_CODES_COUNT; i++)
    {
        if (strcmp(op, OP_CODES[i].opcode) == 0)
        {
            return i;
        }
    }
    return -1;
}

int get_register(const char *reg)
{
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++)
    {
        if (strcmp(reg, REGISTERS[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int is_valid_instruction(const char *inst)
{
    int i;
    for (i = 0; i < INSTRUCTIONS_COUNT; i++)
    {
        if (strcmp(inst, INSTRUCTIONS[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_valid_symbol(const char *symbol, Symbol **symbol_table)
{
    int i;

    if (find_symbol(*symbol_table, symbol))
    {
        fprintf(stderr, "Symbol %s already defined\n", symbol);
        return 0;
    }

    if (get_opcode(symbol) >= 0 || get_register(symbol) >= 0 || is_valid_instruction(symbol))
    {
        fprintf(stderr, "Symbol %s is a reserved word\n", symbol);
        return 0;
    }

    if (strlen(symbol) > MAX_SYMBOL_LENGTH)
    {
        fprintf(stderr, "Symbol %s has illegal name length\n", symbol);
        return 0;
    }

    if (!isalpha(symbol[0]))
    {
        fprintf(stderr, "Symbol %s must start with a letter\n", symbol);
        return 0;
    }

    for (i = 1; i < strlen(symbol); i++)
    {
        if (!isalnum(symbol[i]))
        {
            fprintf(stderr, "Symbol %s must contains only letters or numbers\n", symbol);
            return 0;
        }
    }

    return 1;
}

int parse_data_dir(char *line, int *DC)
{
    char *token;
    int value;

    token = strtok(line, " ");
    while (token)
    {
       
    }
    return 1;
}

int handle_data_or_string(char *line, Symbol **symbol_table, int *DC)
{
    char symbol_name[MAX_SYMBOL_LENGTH + 1];
    char *current = line;
    char *token;

    token = strtok(current, " ");
    if (token && token[strlen(token) - 1] == ':')
    {
        strncpy(symbol_name, token, strlen(token) - 1);
        symbol_name[strlen(token) - 1] = '\0';

        if (is_valid_symbol(symbol_name, symbol_table))
        {
            return create_and_add_symbol(symbol_table, symbol_name, *DC, 0, 1);
        }

        token = strtok(NULL, " ");
    }

    if (token)
    {
        if (strcmp(token, ".data") == 0)
        {
            
            /**
             * parse_data_dir();
             */
        }
        else if (strcmp(token, ".string") == 0)
        {
            /**
             *  parse_string_dir();

             */
        }
        else
        {
            fprintf(stderr, "Unknown or missing directive\n");
            return 0;
        }

        current = strtok(NULL, "");
    }

    return 1;
}

int handle_extern(char *line, Symbol **symbol_table)
{
    char symbol_name[MAX_SYMBOL_LENGTH];
    char extra_forbidden_symbol[MAX_SYMBOL_LENGTH];

    if (sscanf(line, ".extern %s %s", symbol_name, extra_forbidden_symbol) == 1)
    {
        if (is_valid_symbol(symbol_name, symbol_table))
        {
            return create_and_add_symbol(symbol_table, symbol_name, 0, 1, 0);
        }
    }

    return 1;
}

int handle_instruction(char *line, Symbol **symbol_table, int *IC)
{
    char symbol_name[MAX_SYMBOL_LENGTH];
    char *current;

    current = strtok(line, " \t");

    if (current[strlen(current) - 1] == ':')
    {
        strncpy(symbol_name, current, strlen(current) - 1);
        symbol_name[strlen(current) - 1] = '\0';
        current = strtok(NULL, " \t");

        if (is_valid_symbol(symbol_name, symbol_table))
        {
            return create_and_add_symbol(symbol_table, symbol_name, *IC, 0, 0);
        }
    }

    /**
     *  char *current;
    char *operands;
    char *opcode;
    int opcode_index;
    int operands_count = 0;

    current = strtok(line, " \t");
    opcode = current;

    if ((opcode_index = get_opcode(opcode)) == -1)
    {
        fprintf(stderr, "Unknown opcode: %s\n", opcode);
        return 0;
    }

    current = strtok(NULL, " \t");
    operands = current;

    if (operands)
    {
        operands_count = 1;
        current = strtok(NULL, " \t");
        if (current)
        {
            operands_count++;
        }
    }

    if (operands_count != OP_CODES[opcode_index].operands)
    {
        fprintf(stderr, "Invalid number of operands for opcode %s\n", opcode);
        return 0;
    }

    *IC += operands_count + 1;
     */

    return 1;
}
