#ifndef CODE_CONVERSIONS_H
#define CODE_CONVERSIONS_H

#include "../headers/data_struct.h"

void encode_instruction(Instruction *, Machine_Code_Image *, int *);

void update_symbols_in_code_image(Machine_Code_Image *, Symbol *, int);

#endif
