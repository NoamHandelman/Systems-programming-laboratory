#include "../headers/memory.h"
#include "../headers/files.h"
#include "../headers/globals.h"

char *create_file(const char *filename, const char *extension)
{
    char *file_path;
    char *end_pos = strrchr(filename, '.');

    size_t filename_length = end_pos ? (end_pos - filename) : strlen(filename);

    file_path = allocate_memory(filename_length + strlen(extension) + 1);
    if (!file_path)
    {
        return NULL;
    }

    strncpy(file_path, filename, filename_length);
    file_path[filename_length] = '\0';
    strcat(file_path, extension);
    
    return file_path;
}

int clear_empty_lines(const char *filename)
{
    FILE *file;
    FILE *temp;
    char line[MAX_LINE_LENGTH];
    int should_removed;
    int i;

    file = fopen(filename, "r");
    if (!file)
    {
        return 0;
    }
    temp = fopen("temp", "w");
    if (!temp)
    {
        fclose(file);
        return 0;
    }
    while (fgets(line, sizeof(line), file))
    {
        should_removed = 1;
        for (i = 0; line[i] != '\0'; i++)
        {
            if (!isspace(line[i]))
            {
                if (line[i] == ';')
                {
                    break;
                }
                should_removed = 0;
                break;
            }
        }
        if (!should_removed)
        {
            fputs(line, temp);
        }
    }
    fclose(file);
    fclose(temp);
    remove(filename);
    rename("temp", filename);
    return 1;
}
