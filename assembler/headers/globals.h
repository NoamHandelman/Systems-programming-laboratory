#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * The maximum length of a line in the assembly file.
 */

#define MAX_LINE_LENGTH 80

/**
 * The memory size of the machine.
 */

#define MAX_MEMORY_SIZE 4096

/**
 * The maximum length of a symbol.
 */

#define MAX_SYMBOL_LENGTH 31

/**
 * The start of the memory for the assembler.
 */

#define MEMORY_START 100

/**
 * Large enough initial buffer size line.
 */

#define INITIAL_BUFFER_SIZE 1024

/**
 * The addressing mode values.
 */

#define IMMEDIATE 0
#define DIRECT 1
#define INDIRECT_REGISTER 2
#define DIRECT_REGISTER 3

#endif
