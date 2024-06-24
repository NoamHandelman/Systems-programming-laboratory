#include "headers/assembler_transitions.h"

int exec_first_pass(const char *input_filename)
{
    FILE *am_file;
    int IC = 0, DC = 0, should_continue = 1;
    char line[MAX_LINE_LENGTH];

    am_file = fopen(input_filename, "r");
    if (!am_file)
    {
        fprintf(stderr, "Failed to open file %s\n", input_filename);
        return 0;
    }

    while (fgets(line, sizeof(line), am_file) != NULL)
    {
    }

    fclose(am_file);
    if (!should_continue)
    {
        return 0;
    }

    return exec_second_pass(input_filename);
}

int exec_second_pass(const char *input_filename)
{
    return 0;
}
