#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/data_struct.h"
#include "../headers/lexer.h"

void encode_instruction(Instruction *, Machine_Code_Image *, int *);

#endif
