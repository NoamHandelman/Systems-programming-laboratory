#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <ctype.h>
#include "../headers/data_struct.h"

int get_opcode(const char *);

int get_register(const char *);

int is_valid_instruction(const char *);

int is_valid_symbol(const char *, Symbol **);

int handle_data_or_string(char *, Symbol **, int *);

#endif
