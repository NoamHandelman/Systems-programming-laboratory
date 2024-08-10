#ifndef ASSEMBLER_TRANSITIONS_H
#define ASSEMBLER_TRANSITIONS_H

#include <stdio.h>
#include <string.h>
#include "./strings.h"
#include "./globals.h"
#include "./lexer.h"
#include "./code_conversions.h"
#include "./files.h"
#include "./errors.h"
#include "./data_struct.h"

/**
 * @brief The main function for the first pass of the assembler.
 * @return 1 if the first pass was successful, 0 otherwise.
 */

int exec_first_pass(const char *, Macro **);

/**
 * @brief The main function for the second pass of the assembler.
 * @return 1 if the second pass was successful, 0 otherwise.
 */

int exec_second_pass(const char *, Symbol *, Machine_Code_Image *, Machine_Code_Image *, int, int, Declaration *, int);

/**
 * @brief Free all the resources that were allocated during the first and second passes.
 */

void free_all_resources(Symbol *, Declaration *);

#endif
