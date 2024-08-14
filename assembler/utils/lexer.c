#include "../headers/lexer.h"

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
 * @return 1 if the process was successful, 0 otherwise.
 */

int parse_data_dir(char *, int *, Machine_Code_Image *, int, const char *, char *);

/**
 * @brief Function to extract each char from the .string directive and add the asci code to the data image.
 * @return 1 if the process was successful, 0 otherwise.
 */

int parse_string_dir(char *, int *, Machine_Code_Image *, int, const char *, char *);

int get_addressing_mode(const char *);

void validate_instruction(Instruction *, char *, int, const char *, int *);

Instruction *parse_instruction(const char *, char *, int, const char *, int *, Macro **);

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

int is_valid_symbol_in_instruction(const char *symbol, char *line, int line_number, const char *input_filename, Macro **macro_list)
{
    int i, error_found = 1;

    if (strlen(symbol) > MAX_SYMBOL_LENGTH)
    {

        display_error(line, line_number, "Symbol length should be 31 or less", input_filename);
        error_found = 0;
    }

    if (find_macro(*macro_list, symbol))
    {
        display_error(line, line_number, "Symbol can not be a macro name", input_filename);
        error_found = 0;
    }

    if (get_opcode(symbol) >= 0 || get_register(symbol) >= 0 || is_valid_instruction(symbol) || strcmp(symbol, "macr") == 0 || strcmp(symbol, "endmacr") == 0)
    {
        display_error(line, line_number, "Symbol can not be a reserved word", input_filename);
        error_found = 0;
    }

    if (!isalpha(symbol[0]))
    {
        display_error(line, line_number, "Symbol must start with a letter", input_filename);
        error_found = 0;
    }

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

    if (strlen(symbol) > MAX_SYMBOL_LENGTH)
    {

        display_error(line, line_number, "Symbol length should be 31 or less", input_filename);
        error_found = 0;
    }

    if (find_symbol(*symbol_table, symbol))
    {
        display_error(line, line_number, "Symbol already defined", input_filename);
        error_found = 0;
    }

    if (find_macro(*macro_list, symbol))
    {
        display_error(line, line_number, "Symbol can not be a macro name", input_filename);
        error_found = 0;
    }

    if (get_opcode(symbol) >= 0 || get_register(symbol) >= 0 || is_valid_instruction(symbol) || strcmp(symbol, "macr") == 0 || strcmp(symbol, "endmacr") == 0)
    {
        display_error(line, line_number, "Symbol can not be a reserved word", input_filename);
        error_found = 0;
    }

    if (!isalpha(symbol[0]))
    {
        display_error(line, line_number, "Symbol must start with a letter", input_filename);
        error_found = 0;
    }

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

int parse_data_dir(char *line, int *DC, Machine_Code_Image *data_image, int line_number, const char *input_filename, char *full_line)
{
    char *token;
    char *endptr;
    int value;
    int expecting_number = 1;
    int error_found = 1;

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
            value = strtol(token, &endptr, 10);
            if (*endptr == '\0')
            {
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

int parse_string_dir(char *line, int *DC, Machine_Code_Image *data_image, int line_number, const char *input_filename, char *full_line)
{
    char *line_copy;
    int i, error_found = 1;
    char *start_quote, *end_quote;

    printf("initial line : %s\n", line);

    start_quote = strchr(line, '"');
    end_quote = strrchr(line, '"');

    if (!start_quote || !end_quote || start_quote == end_quote)
    {
        display_error(full_line, line_number, "String should start and end with a double quote", input_filename);
        error_found = 0;
    }

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
    if (operand[0] == '#')
    {
        return 0;
    }

    /**
     *  if (operand[0] == '*' && operand[1] == 'r' && strlen(operand) == 3 && isdigit((unsigned char)operand[2]))
        {
            return 2;
        }
     */

    if (operand[0] == '*' && get_register(operand + 1) >= 0)
    {
        return 2;
    }

    /**
     * if (operand[0] == 'r' && strlen(operand) == 2 && isdigit((unsigned char)operand[1]))
    {
        return 3;
    }
     */

    if (get_register(operand) >= 0)
    {
        return 3;
    }

    return 1;
}

int validate_operand(Operand *operand)
{
    if (operand->addressing_mode == 0)
    {
        if (operand->value.num < MIN_IMMEDIATE_VALUE || operand->value.num > MAX_IMMEDIATE_VALUE)
        {
            return 0;
        }
    }

    if (operand->addressing_mode == 2 || operand->addressing_mode == 3)
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

    if (instr->operand_count != OP_CODES[opcode_index].operands)
    {
        display_error(full_line, line_number, "Invalid number of operands", input_filename);
        *should_continue = 0;
    }

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

/**
 * should also valid address mode is ok for each instruction
 */

Instruction *parse_instruction(const char *line, char *full_line, int line_number, const char *input_filename, int *should_continue, Macro **macro_list)
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
            break;
        }

        printf("Token in loop token: %s\n", token);

        if (operand_count == 1)
        {
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
        else if (addressing_mode == 2 || addressing_mode == 3)
        {
            instr->operands[operand_count].value.reg = atoi(token + (addressing_mode == 2 ? 2 : 1));
            printf("Register: %d\n", instr->operands[operand_count].value.reg);
        }
        else
        {
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

    token = strtok(NULL, " ");
    if (token)
    {
        display_error(full_line, line_number, "Extra forbidden characters found in the end of the line", input_filename);
        *should_continue = 0;
    }

    validate_instruction(instr, full_line, line_number, input_filename, should_continue);

    printf("token: %s\n", token);

    return *should_continue ? instr : NULL;
}

/**
 * @brief Main function to process data or string directive.
 * @param line The line to process.
 * @param symbol_table The symbol table.
 * @param DC The data counter.
 * @param data_image The data image array.
 * @param should_continue A flag to indicate if the process should continue.
 * @return 1 if the process was successful, 0 otherwise.
 */

void handle_data_or_string(char *line, Symbol **symbol_table, int *DC, Machine_Code_Image *data_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list)
{
    char symbol_name[MAX_LINE_LENGTH];
    char *current = line;
    char *token;
    char *directive;
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(current, " ");
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
        }
    }
}

void handle_extern(char *line, Symbol **symbol_table, int *externs_count, int *should_continue, int line_number, const char *input_filename, Declaration *entries, Macro **macro_list)
{
    char *token;
    char symbol_name[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(line, " ");

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

            if (is_valid_symbol(symbol_name, symbol_table, original_line, line_number, input_filename, macro_list))
            {
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

            token = strtok(NULL, " ");
            if (token)
            {
                display_error(original_line, line_number, "Only one symbol is allowed after extern declaration", input_filename);
                *should_continue = 0;
            }
        }
        else
        {
            display_error(original_line, line_number, "No symbol provided after extern declaration", input_filename);
            *should_continue = 0;
        }
    }
    else
    {
        display_error(original_line, line_number, "Invalid extern declaration, .extern must be the first token in the line", input_filename);
        *should_continue = 0;
    }
}

void handle_entry(char *line, Symbol **symbol_table, Declaration **entries, int *should_continue, int line_number, const char *input_filename)
{
    char *token;
    char symbol_name[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];

    strncpy(original_line, line, MAX_LINE_LENGTH);

    token = strtok(line, " ");

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
                display_error(original_line, line_number, "Only one symbol is allowed after entry declaration", input_filename);
                *should_continue = 0;
            }
        }
        else
        {
            display_error(original_line, line_number, "No symbol provided after entry declaration", input_filename);
            *should_continue = 0;
        }
    }
    else
    {
        display_error(original_line, line_number, "Invalid entry declaration, .entry must be the first token in the line", input_filename);
        *should_continue = 0;
    }
}

void handle_instruction(char *line, Symbol **symbol_table, int *IC, Machine_Code_Image *code_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list)
{
    char symbol_name[MAX_LINE_LENGTH];
    char *current = line;
    char *token;
    Instruction *instruction;

    char initial_line[MAX_LINE_LENGTH], line_copy[MAX_LINE_LENGTH];
    strncpy(initial_line, line, MAX_LINE_LENGTH);
    strncpy(line_copy, line, MAX_LINE_LENGTH);
    initial_line[MAX_LINE_LENGTH - 1] = '\0';

    token = strtok(current, " ");
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
        instruction = parse_instruction(token, line_copy, line_number, input_filename, should_continue, macro_list);
        if (!instruction)
            return;

        encode_instruction(instruction, code_image, IC);
    }
}
