#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/data_struct.h"
#include "../headers/lexer.h"
#include "../headers/globals.h"

void encode_instruction(Instruction *, Machine_Code_Image *, int *);

void update_symbols_in_code_image(Machine_Code_Image *, Symbol *);

void convert_to_octal(char *, Machine_Code_Image *, int, Machine_Code_Image *, int);

#endif
