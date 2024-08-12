#include "../headers/files.h"

char *create_file(const char *filename, const char *extension)
{
    char *file_path;
    char *end_pos = strrchr(filename, '.');

    size_t filename_length = end_pos ? (end_pos - filename) : strlen(filename);

    file_path = (char *)malloc(filename_length + strlen(extension) + 1);

    if (!file_path)
    {
        return NULL;
    }

    strncpy(file_path, filename, filename_length);
    file_path[filename_length] = '\0';
    strcat(file_path, extension);

    return file_path;
}

void create_ob_file(Machine_Code_Image *code_image, int IC, Machine_Code_Image *data_image, int DC, const char *input_filename)
{
    int i;
    int address = MEMORY_START;
    FILE *output_file;
    char *ob_file_name;
    ob_file_name = create_file(input_filename, ".ob");
    if (!ob_file_name)
    {
        fprintf(stderr, "Failed to create file\n");
    }

    output_file = fopen(ob_file_name, "w");
    if (!output_file)
    {
        fprintf(stderr, "Failed to open file %s\n", ob_file_name);
    }

    fprintf(output_file, "%d %d\n", IC, DC);

    for (i = 0; i < IC; i++)
    {
        fprintf(output_file, "%04d ", address++);
        fprintf(output_file, "%05o\n", code_image[i].value & 0x7FFF);
    }

    for (i = 0; i < DC; i++)
    {
        fprintf(output_file, "%04d ", address++);
        fprintf(output_file, "%05o\n", data_image[i].value & 0x7FFF);
    }

    fclose(output_file);
}

void create_ent_file(Declaration *entries, Symbol *symbol_table, const char *input_filename)
{
    Declaration *current = entries;
    FILE *ent_file;
    char *ent_file_name;
    ent_file_name = create_file(input_filename, ".ent");
    if (!ent_file_name)
    {
        fprintf(stderr, "Failed to create file\n");
    }

    ent_file = fopen(ent_file_name, "w");
    if (!ent_file)
    {
        fprintf(stderr, "Failed to open file %s\n", ent_file_name);
    }

    while (current)
    {
        Symbol *symbol = find_symbol(symbol_table, current->name);
        if (!symbol)
        {
            printf("Entry symbol not found: %s\n", current->name);
        }
        fprintf(ent_file, "%s %04d\n", current->name, symbol->address);
        current = current->next;
    }

    fclose(ent_file);
}

void create_ext_file(Symbol *symbol_table, Machine_Code_Image *code_image, int IC, const char *input_filename)
{
    Symbol *current = symbol_table;
    FILE *ext_file;
    char *ext_file_name;
    int i;
    ext_file_name = create_file(input_filename, ".ext");
    if (!ext_file_name)
    {
        fprintf(stderr, "Failed to create file\n");
    }

    ext_file = fopen(ext_file_name, "w");
    if (!ext_file)
    {
        fprintf(stderr, "Failed to open file %s\n", ext_file_name);
    }

    while (current)
    {
        if (current->is_external)
        {
            for (i = 0; i < IC; i++)
            {
                if (code_image[i].symbol && strcmp(code_image[i].symbol, current->name) == 0)
                {
                    fprintf(ext_file, "%s %04d\n", current->name, i + MEMORY_START);
                }
            }
        }
        current = current->next;
    }

    fclose(ext_file);
}
