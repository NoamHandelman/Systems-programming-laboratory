#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./data_struct.h"
#include "./globals.h"
#include "./strings.h"
#include "./lexer.h"
#include "./files.h"
#include "./errors.h"

/**
 * @brief The main function for the pre proccess stage.
 * @return the path for the am file.
 */

char *exec_preproc(const char *);

/**
 * @brief Handle error in the pre proccess stage.
 */

void *handle_preproc_error(const char *, char *, int, Macro *, char *, FILE *, FILE *);

/**
 * @brief Function to validate macro definition.
 * @return 1 if the macro is valid, 0 otherwise.
 */

int validate_macro(const char *);

#endif
