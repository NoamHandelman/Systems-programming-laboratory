#ifndef FILES_H
#define FILES_H

#include "../headers/data_struct.h"

/**
 * @brief Create a file path with the given filename and extension.
 * @param filename The name of the file.
 * @param extension The extension of the file.
 * @return The file path if the file was created successfully, otherwise return NULL.
 */

char *create_file(const char *filename, const char *extension);

/**
 * @brief Create the object file.
 * @param code_image The code image.
 * @param IC The instruction counter.
 * @param data_image The data image.
 * @param DC The data counter.
 * @param input_filename The name of the input file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

void create_ob_file(Machine_Code_Image *code_image, int IC, Machine_Code_Image_Data *data_image, int DC, const char *input_filename, int *should_continue);

/**
 * @brief Create the entries file.
 * @param entries The list of entries.
 * @param symbol_table The symbol table.
 * @param input_filename The name of the input file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

void create_ent_file(Declaration *entries, Symbol *symbol_table, const char *input_filename, int *should_continue);

/**
 * @brief Create the extern file.
 * @param symbol_table The symbol table.
 * @param code_image The code image.
 * @param IC The instruction counter.
 * @param input_filename The name of the input file.
 * @param should_continue A pointer to a variable that indicates about the proccess failure or success status.
 */

void create_ext_file(Symbol *symbol_table, Machine_Code_Image *code_image, int IC, const char *input_filename, int *should_continue);

#endif
