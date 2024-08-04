#include <string.h>
#include <stdlib.h>
#include "../headers/globals.h"
#include "../headers/data_struct.h"
#include "../headers/lexer.h"
#include "../headers/code_conversions.h"

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

int parse_data_dir(char *, int *, Machine_Code_Image *);

int parse_string_dir(char *, int *, Machine_Code_Image *);

int get_addressing_mode(const char *);

Instruction *parse_instruction(const char *);

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

int parse_data_dir(char *line, int *DC, Machine_Code_Image *data_image)
{
    char *token;
    char *endptr;
    int value;
    int expecting_number = 1;

    if (line[0] == ',' || line[strlen(line) - 1] == ',')
    {
        fprintf(stderr, "Line should not start or end with a comma\n");
    }

    token = strtok(line, " ");
    while (token)
    {
        if (expecting_number)
        {
            value = strtol(token, &endptr, 10);
            if (*endptr == '\0')
            {
                data_image[(*DC)++].value = value;
                printf("Parsed number: %d\n", value);
                expecting_number = 0;
            }
            else
            {
                fprintf(stderr, "Invalid number: %s\n", token);
            }
        }
        else
        {
            if (strcmp(token, ",") == 0)
            {
                expecting_number = 1;
            }
            else
            {
                fprintf(stderr, "Expected comma but found: %s\n", token);
            }
        }
        token = strtok(NULL, " ");
    }

    if (expecting_number)
    {
        fprintf(stderr, "Line should not end with a comma\n");
    }

    return 1;
}

int parse_string_dir(char *line, int *DC, Machine_Code_Image *data_image)
{
    char *line_copy;
    int i;

    printf("initial line : %s\n", line);

    if (line[0] != '"' || line[strlen(line) - 1] != '"')
    {
        fprintf(stderr, "String should start and end with a double quote\n");
    }

    line[strlen(line) - 1] = '\0';
    line_copy = line + 1;

    for (i = 0; i < strlen(line_copy); i++)
    {
        printf("ASCII value of %c: %d\n", line_copy[i], line_copy[i]);
        data_image[(*DC)++].value = line_copy[i];
    }

    printf("End of string sign: 0\n");
    data_image[(*DC)++].value = '\0';
    return 1;
}

int get_addressing_mode(const char *operand)
{
    if (operand[0] == '#')
    {
        return 0;
    }

    if (operand[0] == '*')
    {
        return 2;
    }

    if (operand[0] == 'r')
    {
        return 3;
    }

    return 1;
}

/**
 * should also valid address mode is ok for each instruction
 */

Instruction *parse_instruction(const char *line)
{
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    Instruction *instr;
    int operand_count = 0;

    strncpy(line_copy, line, MAX_LINE_LENGTH);
    line_copy[MAX_LINE_LENGTH - 1] = '\0';

    instr = (Instruction *)malloc(sizeof(Instruction));
    if (!instr)
    {
        return NULL;
    }

    instr->operand_count = 0;

    token = strtok(line_copy, " ");

    if (!token)
    {
        free(instr);
        return NULL;
    }

    if (get_opcode(token) == -1)
    {
        printf("Invalid opcode: %s\n", token);
        free(instr);
        return NULL;
    }

    instr->op_code.opcode = (char *)malloc(strlen(token) + 1);
    if (!instr->op_code.opcode)
    {
        free(instr);
        return NULL;
    }

    strcpy(instr->op_code.opcode, token);

    while ((token = strtok(NULL, " ,")) && operand_count < 2)
    {
        int addressing_mode;
        addressing_mode = get_addressing_mode(token);
        instr->operands[operand_count].addressing_mode = addressing_mode;
        if (addressing_mode == 0)
        {
            instr->operands[operand_count].value.num = atoi(token + 1);
        }
        else if (addressing_mode == 2 || addressing_mode == 3)
        {
            instr->operands[operand_count].value.reg = atoi(token + (addressing_mode == 2 ? 2 : 1));
        }
        else
        {
            instr->operands[operand_count].value.symbol = (char *)malloc(strlen(token) + 1);
            if (!instr->operands[operand_count].value.symbol)
            {
                free(instr->op_code.opcode);
                free(instr);
                return NULL;
            }
            strcpy(instr->operands[operand_count].value.symbol, token);
        }
        operand_count++;
    }
    instr->operand_count = operand_count;

    return instr;
}

int handle_data_or_string(char *line, Symbol **symbol_table, int *DC, Machine_Code_Image *data_image)
{
    char symbol_name[MAX_SYMBOL_LENGTH + 1];
    char *current = line;
    char *token;
    char *directive;

    token = strtok(current, " ");
    if (token && token[strlen(token) - 1] == ':')
    {
        strncpy(symbol_name, token, strlen(token) - 1);
        symbol_name[strlen(token) - 1] = '\0';

        if (is_valid_symbol(symbol_name, symbol_table))
        {
            create_and_add_symbol(symbol_table, symbol_name, *DC, 0, 1);
        }

        token = strtok(NULL, " ");
    }

    if (token)
    {
        directive = token;
        current = strtok(NULL, "");
        if (strcmp(directive, ".data") == 0)
        {
            if (!parse_data_dir(current, DC, data_image))
            {
                fprintf(stderr, "Error parsing .data directive\n");
            }
        }
        else if (strcmp(directive, ".string") == 0)
        {
            if (!parse_string_dir(current, DC, data_image))
            {
                fprintf(stderr, "Error parsing .string directive\n");
            }
        }
        else
        {
            fprintf(stderr, "Unknown or missing directive\n");
        }
    }

    return 1;
}

int handle_extern(char *line, Symbol **symbol_table, int *externs_count)
{
    char symbol_name[MAX_SYMBOL_LENGTH];
    char extra_forbidden_symbol[MAX_SYMBOL_LENGTH];

    if (sscanf(line, ".extern %s %s", symbol_name, extra_forbidden_symbol) == 1)
    {
        if (is_valid_symbol(symbol_name, symbol_table))
        {
            (*externs_count)++;
            return create_and_add_symbol(symbol_table, symbol_name, 0, 1, 0);
        }
        else
        {
            printf("Symbol %s is not valid\n", symbol_name);
        }
    }

    return 1;
}

int handle_entry(char *line, Symbol **symbol_table, Declaration **entries)
{
    char symbol_name[MAX_SYMBOL_LENGTH];
    char extra_forbidden_symbol[MAX_SYMBOL_LENGTH];

    if (sscanf(line, ".entry %s %s", symbol_name, extra_forbidden_symbol) == 1)
    {
        return create_and_add_declaration(entries, symbol_name);
    }
    else
    {
        printf("Invalid entry declaration on line %s\n", line);
    }

    return 1;
}

int handle_instruction(char *line, Symbol **symbol_table, int *IC, Machine_Code_Image *code_image)
{
    char symbol_name[MAX_SYMBOL_LENGTH + 1];
    char *current = line;
    char *token;
    Instruction *instruction;

    char initial_line[MAX_LINE_LENGTH];
    strncpy(initial_line, line, MAX_LINE_LENGTH);
    initial_line[MAX_LINE_LENGTH - 1] = '\0';

    token = strtok(current, " ");
    if (token && token[strlen(token) - 1] == ':')
    {
        strncpy(symbol_name, token, strlen(token) - 1);
        symbol_name[strlen(token) - 1] = '\0';

        if (is_valid_symbol(symbol_name, symbol_table))
        {
            create_and_add_symbol(symbol_table, symbol_name, *IC, 0, 0);
        }

        token = strtok(NULL, "");
    }

    else
    {
        token = initial_line;
    }

    if (token)
    {
        instruction = parse_instruction(token);
        if (!instruction)
        {
            fprintf(stderr, "Error parsing instruction\n");
            return 0;
        }

        encode_instruction(instruction, code_image, IC);
    }

    return 1;
}
