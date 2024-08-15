/**
 * This file bundles the functions needed to parse the syntax of a line in the file
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/lexer.h"
#include "../headers/globals.h"
#include "../headers/code_conversions.h"
#include "../headers/errors.h"

#define MAX_DATA_NUM 16383
#define MIN_DATA_NUM -16384
#define MAX_IMMEDIATE_VALUE 2047
#define MIN_IMMEDIATE_VALUE -2048

OP_CODE OP_CODES[] = {
    {"mov", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"cmp", 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
    {"add", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"sub", 2, {1, 1, 1, 1}, {0, 1, 1, 1}},
    {"lea", 2, {0, 1, 0, 0}, {0, 1, 1, 1}},
    {"clr", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"not", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"inc", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"dec", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"jmp", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"bne", 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"red", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"prn", 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
    {"jsr", 1, {0, 0, 0, 0}, {0, 1, 1, 1}},
    {"rts", 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"stop", 0, {0, 0, 0, 0}, {0, 0, 0, 0}}};

/* Define the registers */
char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Define the instructions */
char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};

/**
 * @brief Function tp extract the numbers from the .data directive and add them to the data image.
 * @param line The line to process.
 * @param DC The data counter.
 * @param data_image The data image array.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param full_line Copy of full line.
 * @return 1 if the process was successful, 0 otherwise.
 */

int parse_data_dir(char *line, int *DC, Machine_Code_Image_Data *data_image, int line_number, const char *input_filename, char *full_line);

/**
 * @brief Function to extract each char from the .string directive and add the asci code to the data image.
 * @param line The line to process.
 * @param DC The data counter.
 * @param data_image The data image array.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param full_line Copy of full line.
 * @return 1 if the process was successful, 0 otherwise.
 */

int parse_string_dir(char *line, int *DC, Machine_Code_Image_Data *data_image, int line_number, const char *input_filename, char *full_line);

/**
 * @brief Function to get the addressing mode of an operand.
 * @param operand The operand to check.
 * @return The addressing mode of the operand.
 */

int get_addressing_mode(const char *operand);

/**
 * @brief Function to check if the instruction is valid.
 * @param inst The instruction to check.
 * @param full_line The full line.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param should_continue A flag to indicate if the process should continue.
 */

void validate_instruction(Instruction *instr, char *full_line, int line_number, const char *input_filename, int *should_continue);

/**
 * @brief Function to parse an instruction line.
 * @param line The line to parse.
 * @param full_line Copy oft the full line.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param should_continue A flag to indicate if the process should continue.
 * @param macro_list The list of macros from the pre proccess.
 * @return The parsed instruction if the operation was successful, otherwise return NULL.
 */

Instruction *parse_instruction(const char *line, char *full_line, int line_number, const char *input_filename, int *should_continue, Macro **macro_list);

/**
 * @brief Function to check if the operand which is symbol in instruction is valid.
 * @param symbol The symbol to check.
 * @param line The line that contains the symbol.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param macro_list The list of macros from the pre proccess.
 * @return 1 if the symbol is valid as operand, 0 otherwise.
 */

int is_valid_symbol_in_instruction(const char *symbol, char *line, int line_number, const char *input_filename, Macro **macro_list);

/**
 * @brief Function to check if the symbol is valid before adding to the symbols table.
 * @param symbol The symbol to check.
 * @param symbol_table The symbol table.
 * @param line The line of the symbol.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param macro_list The list of macros.
 * @return 1 if the symbol is valid, 0 otherwise.
 */

int is_valid_symbol(const char *symbol, Symbol **symbol_table, char *line, int line_number, const char *input_filename, Macro **macro_list);

/**
 * @brief Validate an operand.
 * @param operand The operand to validate.
 * @return 1 if the operand is valid, 0 otherwise.
 */
int validate_operand(Operand *operand);

/**
 * @brief Function to return the index of an opcode.
 */

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

/**
 * @brief Function to return the index of a register.
 */

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

/**
 * @brief Function to check if the instruction is valid.
 */

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

int is_valid_symbol_in_instruction(const char *symbol, char *line, int line_number, const char *input_filename, Macro **macro_list)
{
    int i, error_found = 1;

    /**
     * Check if the symbol is too long.
     */

    if (strlen(symbol) > MAX_SYMBOL_LENGTH)
    {

        display_error(line, line_number, "Symbol length should be 31 or less", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol is already defined as macro.
     */

    if (find_macro(*macro_list, symbol))
    {
        display_error(line, line_number, "Symbol can not be a macro name", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol is a reserved word.
     */

    if (get_opcode(symbol) >= 0 || get_register(symbol) >= 0 || is_valid_instruction(symbol) || strcmp(symbol, "macr") == 0 || strcmp(symbol, "endmacr") == 0)
    {
        display_error(line, line_number, "Symbol can not be a reserved word", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol starts with a letter.
     */

    if (!isalpha(symbol[0]))
    {
        display_error(line, line_number, "Symbol must start with a letter", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol contains only letters or numbers.
     */

    for (i = 1; i < strlen(symbol); i++)
    {
        if (!isalnum(symbol[i]))
        {
            display_error(line, line_number, "Symbol must contains only letters or numbers", input_filename);
            error_found = 0;
        }
    }

    return error_found;
}

int is_valid_symbol(const char *symbol, Symbol **symbol_table, char *line, int line_number, const char *input_filename, Macro **macro_list)
{
    int i, error_found = 1;

    /**
     * Check if the symbol is too long.
     */

    if (strlen(symbol) > MAX_SYMBOL_LENGTH)
    {

        display_error(line, line_number, "Symbol length should be 31 or less", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol is already defined in the symbols table.
     */

    if (find_symbol(*symbol_table, symbol))
    {
        display_error(line, line_number, "Symbol already defined", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol is already defined as macro.
     */

    if (find_macro(*macro_list, symbol))
    {
        display_error(line, line_number, "Symbol can not be a macro name", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol is a reserved word.
     */

    if (get_opcode(symbol) >= 0 || get_register(symbol) >= 0 || is_valid_instruction(symbol) || strcmp(symbol, "macr") == 0 || strcmp(symbol, "endmacr") == 0)
    {
        display_error(line, line_number, "Symbol can not be a reserved word", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol starts with a letter.
     */

    if (!isalpha(symbol[0]))
    {
        display_error(line, line_number, "Symbol must start with a letter", input_filename);
        error_found = 0;
    }

    /**
     * Check if the symbol contains only letters or numbers.
     */

    for (i = 1; i < strlen(symbol); i++)
    {
        if (!isalnum(symbol[i]))
        {
            display_error(line, line_number, "Symbol must contains only letters or numbers", input_filename);
            error_found = 0;
        }
    }

    return error_found;
}

int parse_data_dir(char *line, int *DC, Machine_Code_Image_Data *data_image, int line_number, const char *input_filename, char *full_line)
{
    char *token;
    char *endptr;
    int value;
    int expecting_number = 1;
    int error_found = 1;

    /**
     * Check if the line starts or ends with a comma.
     */

    if (line[0] == ',' || line[strlen(line) - 1] == ',')
    {
        error_found = 0;
        display_error(full_line, line_number, "Line should not start or end with a comma", input_filename);
    }

    token = strtok(line, " ");
    while (token)
    {
        if (expecting_number)
        {
            /**
             * Check if the token is a number
             */

            value = strtol(token, &endptr, 10);
            if (*endptr == '\0')
            {
                /**
                 * Check if the number is in the valid range (number that can be stored in 15 bits)
                 */
                if (value < MIN_DATA_NUM || value > MAX_DATA_NUM)
                {
                    display_error(full_line, line_number, "Number out of valid range", input_filename);
                    error_found = 0;
                }
                else
                {
                    data_image[(*DC)++].value = value;
                    printf("Parsed number: %d\n", value);
                }
                expecting_number = 0;
            }
            else
            {
                error_found = 0;
                expecting_number = 0;
                display_error(full_line, line_number, "Found invalid token (not a an integer)", input_filename);
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
                display_error(full_line, line_number, "Numbers should be separated by comma", input_filename);
                error_found = 0;
            }
        }
        token = strtok(NULL, " ");
    }

    if (expecting_number)
    {
        display_error(full_line, line_number, "Expected number but not found", input_filename);
        error_found = 0;
    }

    return error_found;
}

int parse_string_dir(char *line, int *DC, Machine_Code_Image_Data *data_image, int line_number, const char *input_filename, char *full_line)
{
    char *line_copy;
    int i, error_found = 1;
    char *start_quote, *end_quote;

    printf("initial line : %s\n", line);

    /**
     * Find the first and last double quote in the line.
     */

    start_quote = strchr(line, '"');
    end_quote = strrchr(line, '"');

    /**
     * Check if the string starts and ends with a double quote.
     */

    if (!start_quote || !end_quote || start_quote == end_quote)
    {
        display_error(full_line, line_number, "String should start and end with a double quote", input_filename);
        error_found = 0;
    }

    /**
     * Check if there are unexpected characters before the start and after the end of the string.
     */

    for (i = 0; i < start_quote - line; i++)
    {
        if (!isspace(line[i]))
        {
            display_error(full_line, line_number, "Unexpected characters before the start of the string", input_filename);
            error_found = 0;
            break;
        }
    }

    for (i = end_quote - line + 1; i < strlen(line); i++)
    {
        if (!isspace(line[i]))
        {
            display_error(full_line, line_number, "Unexpected characters after the end of the string", input_filename);
            error_found = 0;
            break;
        }
    }

    line_copy = start_quote + 1;
    while (line_copy < end_quote)
    {
        printf("ASCII value of %c: %d\n", *line_copy, *line_copy);
        if (!isprint(*line_copy))
        {
            display_error(full_line, line_number, "Fount invalid character in string", input_filename);
            error_found = 0;
        }
        else
        {
            data_image[(*DC)++].value = *line_copy++;
        }
    }

    printf("End of string sign: 0\n");
    data_image[(*DC)++].value = '\0';

    return error_found;
}

int get_addressing_mode(const char *operand)
{
    /**
     * Immediate addressing mode
     */
    if (operand[0] == '#')
    {
        return IMMEDIATE;
    }

    /**
     * Indirect register
     */

    if (operand[0] == '*' && get_register(operand + 1) >= 0)
    {
        return INDIRECT_REGISTER;
    }

    /**
     * Direct register
     */

    if (get_register(operand) >= 0)
    {
        return DIRECT_REGISTER;
    }

    /**
     * Direct addressing or invalid addressing mode
     */

    return DIRECT;
}

int validate_operand(Operand *operand)
{
    /**
     * Check if the operand is a number and in the valid range.
     */
    if (operand->addressing_mode == IMMEDIATE)
    {
        if (operand->value.num < MIN_IMMEDIATE_VALUE || operand->value.num > MAX_IMMEDIATE_VALUE)
        {
            return 0;
        }
    }

    /**
     * Check if the operand is a valid register.
     */

    if (operand->addressing_mode == INDIRECT_REGISTER || operand->addressing_mode == DIRECT_REGISTER)
    {
        if (operand->value.reg < 0 || operand->value.reg > 7)
        {
            return 0;
        }
    }
    return 1;
}

void validate_instruction(Instruction *instr, char *full_line, int line_number, const char *input_filename, int *should_continue)
{
    int opcode_index = get_opcode(instr->op_code);

    /**
     * Check if the number of operands is valid.
     */

    if (instr->operand_count != OP_CODES[opcode_index].operands)
    {
        display_error(full_line, line_number, "Invalid number of operands", input_filename);
        *should_continue = 0;
    }

    /**
     * Validate operands locations in the instruction.
     */

    if (instr->operand_count == 1)
    {
        if (!OP_CODES[opcode_index].dest_operands[instr->operands[0].addressing_mode])
        {
            display_error(full_line, line_number, "Invalid destination operand", input_filename);
            *should_continue = 0;
        }

        if (!validate_operand(&instr->operands[0]))
        {
            display_error(full_line, line_number, "Found invalid operand", input_filename);
            *should_continue = 0;
        }
    }

    if (instr->operand_count == 2)
    {
        if (!OP_CODES[opcode_index].src_operands[instr->operands[0].addressing_mode])
        {
            display_error(full_line, line_number, "Invalid source operand", input_filename);
            *should_continue = 0;
        }
        if (!OP_CODES[opcode_index].dest_operands[instr->operands[1].addressing_mode])
        {
            display_error(full_line, line_number, "Invalid destination operand", input_filename);
            *should_continue = 0;
        }

        if (!validate_operand(&instr->operands[1]))
        {
            display_error(full_line, line_number, "Found invalid operand", input_filename);
            *should_continue = 0;
        }
    }
}

Instruction *parse_instruction(const char *line, char *full_line, int line_number, const char *input_filename, int *should_continue, Macro **macro_list)
{
    char line_copy[MAX_LINE_LENGTH];
    char *token = NULL;
    Instruction *instr = NULL;
    int operand_count = 0;

    strncpy(line_copy, line, MAX_LINE_LENGTH);
    line_copy[MAX_LINE_LENGTH - 1] = '\0';

    instr = (Instruction *)malloc(sizeof(Instruction));
    if (!instr)
    {
        display_error(full_line, line_number, "Failed to allocate memory for instruction", input_filename);
        *should_continue = -1;
        return NULL;
    }

    instr->operand_count = 0;

    token = strtok(line_copy, " ");

    if (!token)
    {
        free(instr);
        display_error(full_line, line_number, "No operation code found", input_filename);
        *should_continue = 0;
        return NULL;
    }

    /**
     * Check if the operation code is valid.
     */

    if (get_opcode(token) == -1)
    {
        display_error(full_line, line_number, "Invalid operation code", input_filename);
        *should_continue = 0;
        free(instr);
        return NULL;
    }

    instr->op_code = (char *)malloc(strlen(token) + 1);
    if (!instr->op_code)
    {
        display_error(full_line, line_number, "Failed to allocate memory for operation code", input_filename);
        *should_continue = -1;
        free(instr);
        return NULL;
    }

    strcpy(instr->op_code, token);

    while (operand_count < 2)
    {
        int addressing_mode;
        token = strtok(NULL, " ");

        if (!token)
        {
            /**
             * No following operands found.
             */
            break;
        }

        printf("Token in loop token: %s\n", token);

        if (operand_count == 1)
        {
            /**
             * Check that after the first operand there is a comma, and then the second operand.
             */
            if (strcmp(token, ",") != 0)
            {
                display_error(full_line, line_number, "Expected comma between operands", input_filename);
                *should_continue = 0;
                continue;
            }

            token = strtok(NULL, " ");
            if (!token)
            {
                display_error(full_line, line_number, "Missing second operand after comma", input_filename);
                *should_continue = 0;
                break;
            }
        }

        addressing_mode = get_addressing_mode(token);
        instr->operands[operand_count].addressing_mode = addressing_mode;
        if (addressing_mode == 0)
        {
            instr->operands[operand_count].value.num = atoi(token + 1);
        }
        else if (addressing_mode == INDIRECT_REGISTER || addressing_mode == DIRECT_REGISTER)
        {
            instr->operands[operand_count].value.reg = atoi(token + (addressing_mode == INDIRECT_REGISTER ? 2 : 1));
            printf("Register: %d\n", instr->operands[operand_count].value.reg);
        }
        else
        {
            /**
             * The operand supposed to be a symbol, check if it is valid, if it is not valid the operand is invalid.
             */
            if (!is_valid_symbol_in_instruction(token, full_line, line_number, input_filename, macro_list))
            {
                *should_continue = 0;
                display_error(full_line, line_number, "Invalid operand, not found addressing mode", input_filename);
            }
            else
            {
                instr->operands[operand_count].value.symbol = (char *)malloc(strlen(token) + 1);
                if (!instr->operands[operand_count].value.symbol)
                {
                    display_error(full_line, line_number, "Failed to allocate memory for symbol", input_filename);
                    *should_continue = -1;
                    free(instr->op_code);
                    free(instr);
                    return NULL;
                }
                strcpy(instr->operands[operand_count].value.symbol, token);
            }
        }
        operand_count++;
    }
    instr->operand_count = operand_count;
    printf("operand count: %d\n", instr->operand_count);

    /**
     * Check if there are more then 2 operands.
     */

    token = strtok(NULL, " ");
    if (token)
    {
        display_error(full_line, line_number, "Extra forbidden characters found in the end of the line", input_filename);
        *should_continue = 0;
    }

    /**
     * Validate the instruction, should_continue will be set to 0 if the instruction is invalid.
     */

    validate_instruction(instr, full_line, line_number, input_filename, should_continue);

    printf("token: %s\n", token);

    return *should_continue ? instr : NULL;
}

/**
 * @brief Function to parse a .data or .string line in the file.
 */

void handle_data_or_string(char *line, Symbol **symbol_table, int *DC, Machine_Code_Image_Data *data_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list)
{
    char symbol_name[MAX_LINE_LENGTH];
    char *current = line;
    char *token = NULL;
    char *directive = NULL;
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(current, " ");

    /**
     * Check if the first token is a symbol, if it is a symbol add it to the symbol table.
     */

    if (token && token[strlen(token) - 1] == ':')
    {
        strncpy(symbol_name, token, strlen(token) - 1);
        symbol_name[strlen(token) - 1] = '\0';

        if (!is_valid_symbol(symbol_name, symbol_table, original_line, line_number, input_filename, macro_list))
        {
            *should_continue = 0;
        }
        else
        {
            if (!create_and_add_symbol(symbol_table, symbol_name, *DC, 0, 1))
            {
                display_error(original_line, line_number, "Failed to create and add symbol, memory allocation failed", input_filename);
                *should_continue = -1;
                return;
            };
        }

        token = strtok(NULL, " ");
    }

    if (token)
    {
        directive = token;
        current = strtok(NULL, "");

        /**
         * Check if the directive is valid and parse the line accordingly.
         */

        if (strcmp(directive, ".data") == 0)
        {
            if (!parse_data_dir(current, DC, data_image, line_number, input_filename, original_line))
            {
                *should_continue = 0;
            }
        }

        else if (strcmp(directive, ".string") == 0)
        {
            if (!parse_string_dir(current, DC, data_image, line_number, input_filename, original_line))
            {
                *should_continue = 0;
            }
        }
        else
        {
            display_error(original_line, line_number, "Invalid directive", input_filename);
            *should_continue = 0;
        }
    }
}

void handle_extern(char *line, Symbol **symbol_table, int *externs_count, int *should_continue, int line_number, const char *input_filename, Declaration *entries, Macro **macro_list)
{
    char *token = NULL;
    char symbol_name[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(line, " ");

    /**
     * Check if the first token is a symbol, if it is a symbol ignore it and display warning.
     */

    if (token && token[strlen(token) - 1] == ':')
    {
        display_warning(original_line, line_number, "A symbol before an extern declaration is meaningless", input_filename);
        token = strtok(NULL, " ");
    }

    if (token && strcmp(token, ".extern") == 0)
    {
        token = strtok(NULL, " ");
        if (token)
        {
            strncpy(symbol_name, token, MAX_LINE_LENGTH);
            symbol_name[MAX_LINE_LENGTH - 1] = '\0';

            /**
             * Check if the symbol is valid and add it to the symbol table.
             */

            if (is_valid_symbol_in_instruction(symbol_name, original_line, line_number, input_filename, macro_list))
            {
                /**
                 * Check if the symbol is already declared as entry.
                 */
                if (find_declaration(entries, symbol_name))
                {
                    display_error(original_line, line_number, "Symbol already declared as entry", input_filename);
                    *should_continue = 0;
                }
                else
                {
                    if (!create_and_add_symbol(symbol_table, symbol_name, 0, 1, 0))
                    {
                        display_error(original_line, line_number, "Failed to create and add symbol, memory allocation failed", input_filename);
                        *should_continue = -1;
                        return;
                    }
                    (*externs_count)++;
                }
            }
            else
            {
                *should_continue = 0;
            }

            token = strtok(NULL, " ");
            if (token)
            {
                /**
                 * Check if there are extra characters after the symbol.
                 */
                display_error(original_line, line_number, "Only one symbol is allowed after extern declaration", input_filename);
                *should_continue = 0;
            }
        }
        else
        {
            /**
             * Check for an empty extern declaration.
             */
            display_error(original_line, line_number, "No symbol provided after extern declaration", input_filename);
            *should_continue = 0;
        }
    }
    else
    {
        /**
         * Check if the first token is .extern.
         */
        display_error(original_line, line_number, "Invalid extern declaration, .extern must be the first token in the line", input_filename);
        *should_continue = 0;
    }
}

void handle_entry(char *line, Symbol **symbol_table, Declaration **entries, int *should_continue, int line_number, const char *input_filename)
{
    char *token = NULL;
    char symbol_name[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(line, " ");

    /**
     * Check if the first token is a symbol, if it is a symbol ignore it and display warning.
     */

    if (token && token[strlen(token) - 1] == ':')
    {
        display_warning(original_line, line_number, "A symbol before an entry declaration is meaningless", input_filename);
        token = strtok(NULL, " ");
    }

    if (token && strcmp(token, ".entry") == 0)
    {
        token = strtok(NULL, " ");
        if (token)
        {
            Symbol *symbol;
            strncpy(symbol_name, token, MAX_LINE_LENGTH);
            symbol_name[MAX_LINE_LENGTH - 1] = '\0';

            /**
             * Validate that the symbol not already defined as external.
             */

            symbol = find_symbol(*symbol_table, symbol_name);

            if (symbol && symbol->is_external)
            {
                display_error(original_line, line_number, "Symbol already defined as external", input_filename);
                *should_continue = 0;
            }

            else
            {
                if (!create_and_add_declaration(entries, symbol_name))
                {
                    display_error(original_line, line_number, "Failed to create and add entry declaration, memory allocation failed", input_filename);
                    *should_continue = -1;
                    return;
                }
            }

            token = strtok(NULL, " ");
            if (token)
            {
                /**
                 * Check if there are extra forbidden characters after the symbol.
                 */
                display_error(original_line, line_number, "Only one symbol is allowed after entry declaration", input_filename);
                *should_continue = 0;
            }
        }
        else
        {
            /**
             * Check for an empty entry declaration.
             */
            display_error(original_line, line_number, "No symbol provided after entry declaration", input_filename);
            *should_continue = 0;
        }
    }
    else
    {
        /**
         * Check if the first token is .entry.
         */
        display_error(original_line, line_number, "Invalid entry declaration, .entry must be the first token in the line", input_filename);
        *should_continue = 0;
    }
}

void handle_instruction(char *line, Symbol **symbol_table, int *IC, Machine_Code_Image *code_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list)
{
    char symbol_name[MAX_LINE_LENGTH], initial_line[MAX_LINE_LENGTH], line_copy[MAX_LINE_LENGTH];
    char *current = line, *token = NULL;
    Instruction *instruction = NULL;

    strncpy(initial_line, line, MAX_LINE_LENGTH);
    strncpy(line_copy, line, MAX_LINE_LENGTH);
    initial_line[MAX_LINE_LENGTH - 1] = '\0';

    token = strtok(current, " ");

    /**
     * Check if the first token is a symbol, if it is a valid symbol add it to the symbol table.
     */
    if (token && token[strlen(token) - 1] == ':')
    {
        strncpy(symbol_name, token, strlen(token) - 1);
        symbol_name[strlen(token) - 1] = '\0';

        if (is_valid_symbol(symbol_name, symbol_table, line_copy, line_number, input_filename, macro_list))
        {
            if (!create_and_add_symbol(symbol_table, symbol_name, *IC, 0, 0))
            {
                display_error(line_copy, line_number, "Failed to create and add symbol, memory allocation failed", input_filename);
                *should_continue = -1;
                return;
            };
        }

        token = strtok(NULL, "");
        if (!token)
        {
            /**
             * Validate that there is no line that start with symbol but without any instruction.
             */
            display_error(line_copy, line_number, "No instruction found after symbol", input_filename);
            *should_continue = 0;
            return;
        }
    }

    else
    {
        token = initial_line;
    }

    if (token)
    {
        /**
         * Parse the instruction and encode it.
         */
        instruction = parse_instruction(token, line_copy, line_number, input_filename, should_continue, macro_list);
        if (!instruction)
            return;

        encode_instruction(instruction, code_image, IC);
    }
}
