#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

/**
 * Macro struct
 */
typedef struct Macro
{
    /**
     * The name of the macro.
     */
    char *name;
    /**
     * The lines in the macro
     */
    char **content;
    /**
     * The number of lines in the macro.
     */
    int line_count;
    /**
     * Pointer to the next macro.
     */
    struct Macro *next;
} Macro;

/**
 * @brief Function to create and add a macro to the macro list.
 * @param head The head of the macro list.
 * @param name The name of the macro that need to be added.
 * @return The created macro.
 */

Macro *create_and_add_macro(Macro **head, const char *name);

/**
 * @brief Add line to the macro content.
 * @param macro The macro to add the line to.
 * @param line The line to add to the macro.
 * @return 1 if the line was added successfully, 0 otherwise.
 */

int add_macro_line(Macro *macro, const char *line);

/**
 * @brief Free the memory allocated for the macro list.
 * @param head The head of the macro list.
 */

void free_macros(Macro *head);

/**
 * @brief Find a macro by its name.
 * @param head The head of the macro list.
 * @return The macro if found, NULL otherwise.
 */

Macro *find_macro(Macro *head, const char *name);

/**
 * Symbol struct
 */

typedef struct Symbol
{
    char *name;
    int address;
    int is_external;
    int is_entry;
    int is_data;
    struct Symbol *next;
} Symbol;

/**
 * @brief Function to create and add a symbol to the symbol table.
 * @param symbol_table The symbol table.
 * @param name The name of the symbol.
 * @param address The current address of the symbol.
 * @param is_external Flag to indicate if the symbol is external.
 * @param is_data Flag to indicate if the symbol is data.
 * @return 1 if the symbol was added successfully, 0 otherwise.
 */

int create_and_add_symbol(Symbol **symbol_table, const char *name, int address, int is_external, int is_data);

/**
 * @brief Find a symbol by its name.
 * @param symbol_table The symbol table.
 * @param name The name of the symbol.
 * @return The symbol if found, NULL otherwise.
 */

Symbol *find_symbol(Symbol *symbol_table, const char *name);

/**
 * @brief Free the memory allocated for the symbol table.
 * @param symbol_table The symbol table.
 */

void free_symbol_table(Symbol *symbol_table);

/**
 * remove in the end!!!
 */

void print_symbol_table(Symbol *);

/**
 * @brief Update the addresses of the symbols in the symbol table.
 * @param symbol_table The symbol table.
 * @param IC The current instruction counter.
 */

void update_symbols_addresses(Symbol **symbol_table, int IC);

/**
 * Declaration (entries) struct
 */
typedef struct Declaration
{
    char *name;
    struct Declaration *next;
} Declaration;

/**
 * @brief Function to create and add a declaration to the declaration table.
 * @param table The declaration table.
 * @param name The name of the declaration.
 * @return 1 if the declaration was added successfully, 0 otherwise.
 */

int create_and_add_declaration(Declaration **table, char *name);

/**
 * @brief Free the memory allocated for the declaration table.
 * @param table The declaration table.
 */

void free_declarations(Declaration *table);

/**
 * @brief Find a declaration by its name.
 * @param table The declaration table.
 * @param name The name of the declaration.
 * @return The declaration if found, NULL otherwise.
 */

Declaration *find_declaration(Declaration *table, const char *name);

/**
 * Machine Code Image structures
 */

typedef struct Machine_Code_Image
{
    /**
     * The variable the holds the value of the machine word.
     */
    unsigned short value;
    /**
     * Possible symbol.
     */
    char *symbol;
    int line_number;
} Machine_Code_Image;

typedef struct Machine_Code_Image_Data
{
    unsigned short value;
} Machine_Code_Image_Data;

/**
 * @brief Free the memory allocated for the machine code image.
 * @param code_image The machine code image.
 * @param IC The current instruction counter.
 */

void free_machine_code_image(Machine_Code_Image *code_image, int IC);

/**
 * Instruction structures
 */

/**
 * The structure that holds the opcode information.
 */
typedef struct OP_CODE
{
    /**
     * The opcode name
     */
    char *opcode;
    /**
     * The number of the operands.
     */
    int operands;
    /**
     * The allowed addressing modes for the source operands.
     */
    int src_operands[4];
    /**
     * The allowed addressing modes for the destination operands.
     */
    int dest_operands[4];
} OP_CODE;

/**
 * The structure that holds the operand information.
 */
typedef struct Operand
{

    /**
     * The value of the operand.
     */
    union
    {
        int reg;
        int num;
        char *symbol;
    } value;

    int addressing_mode;

} Operand;

typedef struct Instruction
{
    char *op_code;
    Operand operands[2];
    int operand_count;
} Instruction;

/**
 * @brief Free the memory allocated for the instruction struct.
 * @param instruction The instruction to free.
 */

void free_instruction(Instruction *instruction);

#endif
