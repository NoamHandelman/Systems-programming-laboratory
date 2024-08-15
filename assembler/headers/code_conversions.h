#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include "../headers/data_struct.h"

/**
 * @brief Converts a instruction to machine words.
 * @param instruction The current parsed instruction.
 * @param code_image The code image array.
 * @param IC The instruction counter.
 */

void encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC);

/**
 * @brief Update the symbols in the code image according the symbols table.
 * @param code_image The code image array.
 * @param symbol_table The symbol table.
 * @param IC The instruction counter.
 */

void update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC);

#endif
