#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include "../headers/data_struct.h"

/**
 * @brief Converts a instruction to machine words.
 * @param instruction The current parsed instruction.
 * @param code_image The code image array.
 * @param IC The instruction counter.
 */

int encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC, char *line_copy, int line_number, const char *input_filename);

/**
 * @brief Update the ARE bits of the symbols in the code image array.
 * @param code_image The code image array.
 * @param symbol_table The symbol table.
 * @param IC The instruction counter.
 * @param input_filename The name of the input file.
 * @return 1 if the symbols were updated successfully, 0 otherwise.
 */

int update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC, const char *input_filename);

#endif
