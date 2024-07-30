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

char *exec_preproc(const char *);

void *handle_preproc_error(const char *, Macro *, char *, FILE *, FILE *);

int validate_macro(const char *);

#endif
