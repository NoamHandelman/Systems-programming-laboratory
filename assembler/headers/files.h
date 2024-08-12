#ifndef FILES_H
#define FILES_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "./globals.h"
#include "./data_struct.h"

/**
 * @brief Create a file path with the given filename and extension.
 * @return the newly created file path.
 */
char *create_file(const char *, const char *);

void create_ob_file(Machine_Code_Image *, int, Machine_Code_Image *, int, const char *);

void create_ent_file(Declaration *, Symbol *, const char *);

void create_ext_file(Symbol *, Machine_Code_Image *, int, const char *);

#endif
