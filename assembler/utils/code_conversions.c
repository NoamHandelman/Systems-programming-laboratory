#include "../headers/code_conversions.h"
#include "../headers/globals.h"

#define MACHINE_WORD_SIZE 15

unsigned short encode_addressing_mode(int addressing_mode)
{
    unsigned short word = 0;
    if (addressing_mode == 0)
        word = 1;
    else if (addressing_mode == 1)
        word = 2;
    else if (addressing_mode == 2)
        word = 4;
    else if (addressing_mode == 3)
        word = 8;
    return word;
}

void encode_instruction(Instruction *instruction, Machine_Code_Image *code_image, int *IC)
{
    int opcode_index = get_opcode(instruction->op_code.opcode);
    int i;
    int j;

    code_image[*IC].value = 0;

    code_image[*IC].value |= (opcode_index << 11);

    if (instruction->operand_count > 0)
    {
        if (instruction->operand_count == 1)
        {
            code_image[*IC].value |= (encode_addressing_mode(instruction->operands[0].addressing_mode) << 3);
        }
        else
        {
            code_image[*IC].value |= (encode_addressing_mode(instruction->operands[0].addressing_mode) << 7);
        }
    }

    if (instruction->operand_count > 1)
    {
        code_image[*IC].value |= (encode_addressing_mode(instruction->operands[1].addressing_mode) << 3);
    }

    code_image[*IC].value |= (1 << 2);

    printf("Machine word value: ");
    for (i = 0; i < MACHINE_WORD_SIZE; i++)
    {
        printf("%d", (code_image[*IC].value >> (MACHINE_WORD_SIZE - 1 - i)) & 1);
    }
    printf("\n");

    (*IC)++;
    printf("Done processing 1 word of instruction\n");

    for (i = 0; i < instruction->operand_count; i++)
    {
        code_image[*IC].value = 0;
        code_image[*IC].symbol = NULL;

        if (instruction->operands[i].addressing_mode == 0)
        {
            code_image[*IC].value |= (instruction->operands[i].value.num << 3);
        }
        else if (instruction->operands[i].addressing_mode == 1)
        {
            code_image[*IC].symbol = instruction->operands[i].value.symbol;
        }
        else if (instruction->operands[i].addressing_mode == 2 || instruction->operands[i].addressing_mode == 3)
        {
            if (instruction->operand_count == 2)
            {
                if ((instruction->operands[0].addressing_mode == 2 || instruction->operands[0].addressing_mode == 3) && (instruction->operands[1].addressing_mode == 2 || instruction->operands[1].addressing_mode == 3))
                {
                    printf("Both operands are registers\n");
                    code_image[*IC].value |= (instruction->operands[0].value.reg << 6);
                    code_image[*IC].value |= (instruction->operands[1].value.reg << 3);
                    code_image[*IC].value |= (1 << 2);
                    printf("Machine word value: ");
                    for (j = 0; j < MACHINE_WORD_SIZE; j++)
                    {
                        printf("%d", (code_image[*IC].value >> (MACHINE_WORD_SIZE - 1 - j)) & 1);
                    }
                    printf("\n");

                    (*IC)++;
                    printf("Done processing %d word of instruction\n", i + 2);
                    break;
                }
                else
                {
                    if (i == 0)
                    {
                        printf("First operand is register : %d\n", instruction->operands[i].value.reg);
                        code_image[*IC].value |= (instruction->operands[i].value.reg << 6);
                    }
                    else
                    {
                        code_image[*IC].value |= (instruction->operands[i].value.reg << 3);
                    }
                }
            }
            else
            {
                printf("There is only one operand and is register\n");
                code_image[*IC].value |= (instruction->operands[i].value.reg << 3);
            }
        }
        code_image[*IC].value |= (1 << 2);
        printf("Machine word value: ");
        for (j = 0; j < MACHINE_WORD_SIZE; j++)
        {
            printf("%d", (code_image[*IC].value >> (MACHINE_WORD_SIZE - 1 - j)) & 1);
        }
        printf("\n");

        (*IC)++;
        printf("Done processing %d word of instruction\n", i + 2);
    }
}

void update_symbols_in_code_image(Machine_Code_Image *code_image, Symbol *symbol_table)
{
    int i;
    Symbol *current = symbol_table;
    while (current)
    {
        for (i = 0; i < 2048; i++)
        {
            if (code_image[i].symbol)
            {
                if (strcmp(code_image[i].symbol, current->name) == 0)
                {
                    code_image[i].value |= (current->address << 3);

                    if (current->is_entry)
                    {
                        code_image[i].value |= (1 << 1);
                        code_image[i].value &= ~(1 << 2);
                    }
                }
            }
        }
        current = current->next;
    }
}
