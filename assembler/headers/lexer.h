#ifndef LEXER_H
#define LEXER_H

#include "../headers/data_struct.h"

/**
 * @brief Get the opcode index of the instruction.
 * @param op the opcode to get the index of.
 * @return the opcode index of the instruction if found, otherwise return -1.
 */

int get_opcode(const char *op);

/**
 * @brief Get the register index.
 * @param reg the register to get the index of.
 * @return the register index if found, otherwise return -1.
 */

int get_register(const char *reg);

/**
 * @brief Check if the instruction is valid.
 * @param inst the instruction word to check.
 * @return 1 if the instruction is valid, 0 otherwise.
 */

int is_valid_instruction(const char *inst);

/**
 * @brief This function parse the line and handle the data or string directives.
 * @param line The line to parse.
 * @param symbol_table The symbol table.
 * @param DC The data counter.
 * @param data_image The data image array.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param macro_list The list of macros.
 */

void handle_data_or_string(char *line, Symbol **symbol_table, int *DC, Machine_Code_Image_Data *data_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list);

/**
 * @brief This function handles with declaration of an extern symbol.
 * @param line The line to parse.
 * @param symbol_table The symbol table.
 * @param externs_count The number of externs declarations in the file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param entries The list of entries.
 * @param macro_list The list of macros.
 */

void handle_extern(char *line, Symbol **symbol_table, int *externs_count, int *should_continue, int line_number, const char *input_filename, Declaration *entries, Macro **macro_list);

/**
 * @brief This function handles with declaration of an entry symbol.
 * @param line The line to parse.
 * @param symbol_table The symbol table.
 * @param entries The list of entries.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 */

void handle_entry(char *line, Symbol **symbol_table, Declaration **entries, int *should_continue, int line_number, const char *input_filename);

/**
 * @brief This function handles with instruction line.
 * @param line The line to parse.
 * @param symbol_table The symbol table.
 * @param IC The instruction counter.
 * @param code_image The code image array.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param macro_list The list of macros.
 */

void handle_instruction(char *line, Symbol **symbol_table, int *IC, Machine_Code_Image *code_image, int *should_continue, int line_number, const char *input_filename, Macro **macro_list);

/**
 * @brief Function to check if operand which is symbol in instruction, or macro name are valid (Function to validate symbol and macro definition).
 * @param symbol The symbol or macro to check.
 * @param line The line that contains the symbol or macro.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param macro_list The list of macros from the pre proccess.
 * @return 1 if the symbol or macro are valid, 0 otherwise.
 */

int is_valid_symbol_in_instruction(const char *symbol, char *line, int line_number, const char *input_filename, Macro **macro_list);

#endif
