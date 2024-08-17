#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include "../headers/data_struct.h"

/**
 * @brief Converts a instruction to machine words.
 * @param instruction The current parsed instruction.
 * @param code_image The code image array.
 * @param IC The instruction counter.
 * @param line_copy The copy of the current line.
 * @param line_number The current line number.
 * @param input_filename The name of the input file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

void encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC, char *line_copy, int line_number, const char *input_filename, int *should_continue);

/**
 * @brief Update the ARE bits of the symbols in the code image array.
 * @param code_image The code image array.
 * @param symbol_table The symbol table.
 * @param IC The instruction counter.
 * @param input_filename The name of the input file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

void update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC, const char *input_filename, int *should_continue);

#endif
