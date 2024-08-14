#ifndef ASSEMBLER_TRANSITIONS_H
#define ASSEMBLER_TRANSITIONS_H

#include <stdio.h>
#include <string.h>
#include "./files.h"
#include "./errors.h"
#include "./data_struct.h"

/**
 * @brief Execute the first pass of the assembler.
 * @param input_filename The name of the am file to process.
 * @param macro_list The list of macros that collected in the pre process stage.
 * @return 1 If the file has been processed successfully , 0 if the errors found , or -1 if errors were detected that require exiting the program .
 */

int exec_first_pass(const char *input_filename, Macro **macro_list);

/**
 * @brief Execute the second pass of the assembler.
 * @param input_filename The name of the am file to process.
 * @param symbol_table The symbol table that was created in the first pass.
 * @param code_image The code image that was created in the first pass.
 * @param data_image The data image that was created in the first pass.
 * @param IC The instruction counter that was created in the first pass.
 * @param DC The data counter that was created in the first pass.
 * @param entries The list of entries that were found in the first pass.
 * @param externs_count The number of externs that were found in the first pass.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

int exec_second_pass(const char *input_filename, Symbol *symbol_table, Machine_Code_Image *code_image, Machine_Code_Image *data_image, int IC, int DC, Declaration *entries, int externs_count, int *should_continue);

/**
 * @brief Free memory of resources that were allocated during the first and second passes.
 * @param symbol_table The symbol table.
 * @param entries The list of entries.
 * @param code_image The code image.
 * @param IC The instruction counter.
 */

void free_all_resources(Symbol *symbol_table, Declaration *entries, Machine_Code_Image *code_image, int IC);

#endif
