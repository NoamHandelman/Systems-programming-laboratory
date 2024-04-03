/* This program simulates the addition of two binary numbers using bitwise operations
by iterating through each bit of the input numbers, calculating the sum and carry for each bit position,
and accumulating the result in a binary format.
The program prompts the user to enter two non-negative integers, representing the binary numbers to be added.
The binary and decimal representation of the sum of the two input numbers. */

#include <stdio.h>

/* Print base 2 number */
unsigned int printInBase2(unsigned int);

/* Preform addition of two binary numbers using bitwise operations */
unsigned int my_add(unsigned int, unsigned int);

unsigned int printInBase2(unsigned int number)
{
    int j = 0;

    for (j = 31; j >= 0; j--)
    {
        printf("%u", (number >> j) & 1);
    }

    return 0;
}

unsigned int my_add(unsigned int a, unsigned int b)
{
    unsigned int carry = 0, result = 0, originalA = a, originalB = b; /* Carry, end result and original values*/
    int i = 0;                                                        /* Bit position */

    while (a != 0 || b != 0)
    {
        unsigned int aBit = a & 1; /* Rightmost bit of a */
        unsigned int bBit = b & 1; /* Rightmost bit of b */
        unsigned int sum = aBit ^ bBit ^ carry;
        carry = (aBit & bBit) | (aBit & carry) | (bBit & carry);

        result |= (sum << i);

        a >>= 1;
        b >>= 1;
        i++;
    }

    if (carry)
    {
        result |= (carry << i);
    }

    printf("Operand A in 2 base: ");
    printInBase2(originalA);

    printf("\nOperand B in 2 base: ");
    printInBase2(originalB);

    printf("\nResult in 2 base: ");
    printInBase2(result);

    printf("\n%u", result);

    return result;
}

int main()
{
    unsigned int a, b; /* a and b operands */

    printf("Please enter two non-negative integers (up to 6 digits): ");
    scanf("%u %u", &a, &b);

    printf("You entered:\nOperand A in 10 base = %u\nOperand B in 10 base = %u\n", a, b);

    printf("\nResult in base 10: %u\n", my_add(a, b));

    return 0;
}
