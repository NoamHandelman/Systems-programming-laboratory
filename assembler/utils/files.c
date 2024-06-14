#include "../headers/memory.h"
#include "../headers/files.h"

char *create_file(const char *filename, const char *extension)
{
    char *file_path;
    file_path = allocate_memory(strlen(filename) + strlen(extension) + 1);
    if (!file_path)
    {
        return NULL;
    }
    strcpy(file_path, filename);
    strcat(file_path, extension);
    return file_path;
}
