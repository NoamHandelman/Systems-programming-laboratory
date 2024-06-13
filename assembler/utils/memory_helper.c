#include "../headers/memory_helper.h"

void *allocate_memory(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    return ptr;
}
