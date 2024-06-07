#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 28
#define MAX_FILENAME 100

/*Structures for linked list node and hash table*/
typedef struct Node
{
    char filename[MAX_FILENAME];
    int count;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} HashTable;

void init_table(HashTable *);
void add_count(HashTable *, int, const char *);
void read_file(HashTable *, const char *);
void print_table(HashTable *);
void free_table(HashTable *);

void init_table(HashTable *table)
{
    int i;
    for (i = 0; i <= MAX_VALUE; i++)
    {
        table[i].head = NULL;
    }
}

void add_count(HashTable *table, int value, const char *filename)
{
    Node *current = table[value].head;
    Node *prev = NULL;
    Node *new_node = NULL;

    while (current != NULL)
    {
        if (strcmp(current->filename, filename) == 0)
        {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }

    /*Create a new node if filename is not found*/
    new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(1);
    }
    strcpy(new_node->filename, filename);
    new_node->count = 1;
    new_node->next = NULL;

    if (prev == NULL)
    {
        table[value].head = new_node;
    }
    else
    {
        prev->next = new_node;
    }
}

void read_file(HashTable *table, const char *filename)
{
    int num;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", filename);
        exit(1);
    }

    while (fscanf(file, "%d", &num) != EOF)
    {
        if (num >= 0 && num <= MAX_VALUE)
        {
            add_count(table, num, filename);
        }
    }

    fclose(file);
}

void print_table(HashTable *table)
{
    int i;
    for (i = 0; i <= MAX_VALUE; i++)
    {
        Node *current = table[i].head;
        if (current != NULL)
        {
            printf("%d appears in", i);
            while (current != NULL)
            {
                printf(" file %s - %d time%s", current->filename, current->count, (current->count > 1) ? "s" : "");
                current = current->next;
                if (current != NULL)
                {
                    printf(", ");
                }
            }
            printf("\n");
        }
    }
}

void free_table(HashTable *table)
{
    int i;
    for (i = 0; i <= MAX_VALUE; i++)
    {
        Node *current = table[i].head;
        while (current != NULL)
        {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
        table[i].head = NULL;
    }
}

int main(int argc, char *argv[])
{
    HashTable table[MAX_VALUE + 1];
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "No input files specified.\n");
        return 1;
    }

    init_table(table);

    for (i = 1; i < argc; i++)
    {
        read_file(table, argv[i]);
    }

    print_table(table);
    free_table(table);

    return 0;
}
