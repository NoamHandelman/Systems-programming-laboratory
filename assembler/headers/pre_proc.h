#include <stdio.h>
#include <string.h>
#include "./data_struct.h"

char *exec_preproc(const char *);

Macro *create_macro(const char *);

void add_macro_line(Macro *, const char *);

void add_macro(Macro **, Macro *);

void free_macros(Macro *);

Macro *find_macro(Macro *, const char *);

void *handle_preproc_error(const char *, Macro *, char *, FILE *, FILE *);

int validate_macro(const char *);
