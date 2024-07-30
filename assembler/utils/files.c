#include "../headers/files.h"
#include "../headers/globals.h"

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
