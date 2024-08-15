/**
 * This files bundles the functions that are responsible for encoding the instructions and the symbols into machine code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/code_conversions.h"
#include "../headers/lexer.h"
#include "../headers/globals.h"

#define THREE_SHIFT << 3
#define SIX_SHIFT << 6
#define THIRD_BIT_MASK (1 << 2)

/**
 * @brief Convert the addressing mode to a bitmask to encode the operand.
 * @param addressing_mode The addressing mode to convert.
 * @return The bitmask of the addressing mode.
 */

unsigned short convert_addressing_mode_to_bitmask(int addressing_mode);

unsigned short convert_addressing_mode_to_bitmask(int addressing_mode)
{
    unsigned short mask = 0;
    if (addressing_mode == IMMEDIATE)
        mask = 1;
    else if (addressing_mode == DIRECT)
        mask = 2;
    else if (addressing_mode == INDIRECT_REGISTER)
        mask = 4;
    else if (addressing_mode == DIRECT_REGISTER)
        mask = 8;
    return mask;
}

void encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC)
{
    int opcode_index = get_opcode(instruction->op_code);
    int i;

    code_image[*IC].value = 0;
    code_image[*IC].symbol = NULL;

    code_image[*IC].value |= (opcode_index << 11);

    if (instruction->operand_count > 0)
    {
        if (instruction->operand_count == 1)
        {
            code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[0].addressing_mode) THREE_SHIFT);
        }
        else
        {
            code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[0].addressing_mode) << 7);
        }
    }

    if (instruction->operand_count > 1)
    {
        code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[1].addressing_mode) THREE_SHIFT);
    }

    code_image[(*IC)++].value |= THIRD_BIT_MASK;

    for (i = 0; i < instruction->operand_count; i++)
    {
        int addressing_mode = instruction->operands[i].addressing_mode;

        if (addressing_mode == IMMEDIATE)
        {
            code_image[*IC].value |= (instruction->operands[i].value.num THREE_SHIFT);
        }
        else if (addressing_mode == DIRECT)
        {
            code_image[*IC].symbol = instruction->operands[i].value.symbol;
        }
        else if (addressing_mode == INDIRECT_REGISTER || addressing_mode == DIRECT_REGISTER)
        {
            if (instruction->operand_count == 2)
            {
                if ((instruction->operands[0].addressing_mode == INDIRECT_REGISTER || instruction->operands[0].addressing_mode == DIRECT_REGISTER) && (instruction->operands[1].addressing_mode == INDIRECT_REGISTER || instruction->operands[1].addressing_mode == DIRECT_REGISTER))
                {
                    code_image[*IC].value |= (instruction->operands[0].value.reg SIX_SHIFT);
                    code_image[*IC].value |= (instruction->operands[1].value.reg THREE_SHIFT);
                    code_image[(*IC)++].value |= THIRD_BIT_MASK;
                    break;
                }
                else
                {
                    if (i == 0)
                    {
                        printf("First operand is register : %d\n", instruction->operands[i].value.reg);
                        code_image[*IC].value |= (instruction->operands[i].value.reg SIX_SHIFT);
                    }
                    else
                    {
                        code_image[*IC].value |= (instruction->operands[i].value.reg THREE_SHIFT);
                    }
                }
            }
            else
            {
                printf("There is only one operand and is register\n");
                code_image[*IC].value |= (instruction->operands[i].value.reg THREE_SHIFT);
            }
        }
        code_image[(*IC)++].value |= THIRD_BIT_MASK;
    }
}

void update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC)
{
    int i;
    Symbol *current = symbol_table;
    while (current)
    {
        for (i = 0; i < IC; i++)
        {
            if (code_image[i].symbol)
            {
                if (strcmp(code_image[i].symbol, current->name) == 0)
                {
                    code_image[i].value |= (current->address THREE_SHIFT);

                    if (current->is_entry)
                    {
                        code_image[i].value |= (1 << 1);
                        code_image[i].value &= ~THIRD_BIT_MASK;
                    }

                    if (current->is_external)
                    {
                        code_image[i].value |= (1 << 0);
                        code_image[i].value &= ~THIRD_BIT_MASK;
                    }
                }
            }
        }
        current = current->next;
    }
}
