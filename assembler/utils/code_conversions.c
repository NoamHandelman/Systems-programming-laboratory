/**
 * This files bundles the functions that are responsible for encoding the instructions and the symbols into machine code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/code_conversions.h"
#include "../headers/globals.h"
#include "../headers/errors.h"
#include "../headers/data_struct.h"
#include "../headers/lexer.h"

#define THREE_SHIFT << 3
#define SIX_SHIFT << 6
#define THIRD_BIT_MASK (1 << 2)

/**
 * @brief Convert the addressing mode to a bitmask to encode the operand.
 * @param addressing_mode The addressing mode to convert.
 * @return The bitmask of the addressing mode.
 */

unsigned short convert_addressing_mode_to_bitmask(int addressing_mode);

/**
 * Inital masking for the operand addressing mode.
 */

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

int encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC, char *line_copy, int line_number, const char *input_filename)
{
    /**
     * Get the opcode index of the instruction.
     */
    int opcode_index = get_opcode(instruction->op_code);
    int i;

    /**
     * Encode the opcode according its index in bits 11-14.
     */

    code_image[*IC].value |= (opcode_index << 11);

    /**
     * Encode the first word by checking the number of operands.
     */

    if (instruction->operand_count > 0)
    {
        if (instruction->operand_count == 1)
        {
            /**
             * If there is only one operand, encode the addressing mode in bits 3-6.
             */
            code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[0].addressing_mode) THREE_SHIFT);
        }
        else
        {
            /**
             * If there are two operands, encode the addressing mode of the first operand in bits 7-10.
             */
            code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[0].addressing_mode) << 7);
        }
    }

    if (instruction->operand_count > 1)
    {
        /**
         * If there are two operands, encode the addressing mode of the second operand in bits 3-6.
         */
        code_image[*IC].value |= (convert_addressing_mode_to_bitmask(instruction->operands[1].addressing_mode) THREE_SHIFT);
    }

    code_image[*IC].line_number = line_number;
    code_image[(*IC)++].value |= THIRD_BIT_MASK;

    /**
     * Encode second and third words if there are operands by iterating the operand of the instruction.
     */

    for (i = 0; i < instruction->operand_count; i++)
    {
        int addressing_mode = instruction->operands[i].addressing_mode;

        code_image[*IC].line_number = line_number;

        if (addressing_mode == IMMEDIATE)
        {
            /**
             * If the addressing mode is immediate, encode its value in bits 3-14.
             */
            code_image[*IC].value |= (instruction->operands[i].value.num THREE_SHIFT);
        }
        else if (addressing_mode == DIRECT)
        {
            /**
             * If the addressing mode is direct, keep the symbol name so we will can encode its address in the second pass
             */
            code_image[*IC].symbol = malloc(strlen(instruction->operands[i].value.symbol) + 1);
            if (code_image[*IC].symbol)
            {
                strcpy(code_image[*IC].symbol, instruction->operands[i].value.symbol);
            }
            else
            {
                display_error(line_copy, line_number, "Failed to allocate memory for the symbol in the code image", input_filename);
                free_instruction(instruction);
                return -1;
            }
        }
        else if (addressing_mode == INDIRECT_REGISTER || addressing_mode == DIRECT_REGISTER)
        {
            if (instruction->operand_count == 2)
            {
                if ((instruction->operands[0].addressing_mode == INDIRECT_REGISTER || instruction->operands[0].addressing_mode == DIRECT_REGISTER) && (instruction->operands[1].addressing_mode == INDIRECT_REGISTER || instruction->operands[1].addressing_mode == DIRECT_REGISTER))
                {
                    /**
                     * Check if both operands are registers and encode the operands in one machine word.
                     */
                    code_image[*IC].value |= (instruction->operands[0].value.reg SIX_SHIFT);
                    code_image[*IC].value |= (instruction->operands[1].value.reg THREE_SHIFT);
                    code_image[(*IC)++].value |= THIRD_BIT_MASK;
                    break;
                }
                else
                {
                    /**
                     * If there are two operands and one of them is register, encode the register in its place.
                     */
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
                /**
                 * There is only one operand and is register.
                 */
                printf("There is only one operand and is register\n");
                code_image[*IC].value |= (instruction->operands[i].value.reg THREE_SHIFT);
            }
        }
        /**
         * Set initial ARE value for all machine words
         */
        code_image[(*IC)++].value |= THIRD_BIT_MASK;
    }
    /**
     * Free the instruction structure after encoding it.
     */
    free_instruction(instruction);
    return 1;
}

/**
 * Iterate over the symbol table, if the current symbol is found in the code image array update its value with the current symbol address.
 * If the symbol is an entry, set the second bit of the value to 1.
 * If the symbol is external, set the first bit of the value to 1.
 */

/**
 * void update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC)
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
 */

/**
 * Iterate over code image, if the current entry has symbol in it is and this symbol 
 * is found in the symbol data then update its the value in the code image according 
 * the address of the symbol that found.
 * If the symbol is an entry, set the second bit of the value to 1.
 * If the symbol is external, set the first bit of the value to 1.
 * if the symbol is not found in the symbol table, display an error and return 0.
 */


int update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table, int IC, const char *input_filename)
{
    int i;
    for (i = 0; i < IC; i++)
    {
        if (code_image[i].symbol)
        {
            Symbol *symbol = find_symbol(symbol_table, code_image[i].symbol);
            if (symbol)
            {
                code_image[i].value |= (symbol->address THREE_SHIFT);
                if (symbol->is_entry)
                {
                    code_image[i].value |= (1 << 1);
                    code_image[i].value &= ~THIRD_BIT_MASK;
                }
                if (symbol->is_external)
                {
                    code_image[i].value |= (1 << 0);
                    code_image[i].value &= ~THIRD_BIT_MASK;
                }
            }
            else
            {
                display_error(code_image[i].symbol, code_image[i].line_number, "Undefined symbol", input_filename);
                return 0;
            }
        }
    }
    return 1;
}
