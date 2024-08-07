#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <ctype.h>
#include "../headers/data_struct.h"

int get_opcode(const char *);

int get_register(const char *);

int is_valid_instruction(const char *);

int is_valid_symbol(const char *, Symbol **, char *, int, const char *);

int handle_data_or_string(char *, Symbol **, int *, Machine_Code_Image *, int *, int, const char *);

int handle_extern(char *, Symbol **, int *, int *, int, const char *);

int handle_entry(char *, Symbol **, Declaration **);

int handle_instruction(char *, Symbol **, int *, Machine_Code_Image *, int *, int, const char *);

#endif
