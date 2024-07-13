#ifndef STRINGS_H
#define STRINGS_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>

int check_for_extra_chars(char *);

int is_empty_line(const char *);

void handle_spaces(char *);

#endif
