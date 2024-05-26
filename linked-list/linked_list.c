#include <stdlib.h>
#include <stdio.h>

typedef struct node *ptr;

typedef struct node
{
    int data;
    ptr next;
} node;

void add_node(ptr *, int);
void print_list(ptr);
void free_list(ptr *);

int main()
{
    ptr head = NULL;
    int num;

    while (scanf("%d", &num) == 1)
        add_node(&head, num);

    print_list(head);

    free_list(&head);

    return 0;
}

void add_node(ptr *head, int data)
{
    ptr p1, p2;
    ptr new_node = (ptr)malloc(sizeof(node));

    if (!new_node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->data = data;
    p1 = *head;
    while (p1 && p1->data < data)
    {
        p2 = p1;
        p1 = p1->next;
    }

    if (p1 == *head)
    {
        new_node->next = *head;
        *head = new_node;
    }
    else
    {
        p2->next = new_node;
        new_node->next = p1;
    }
}

void free_list(ptr *head)
{
    ptr temp;
    while (*head)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void print_list(ptr head)
{
    while (head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}
