#include "set.h"

int get_set_index(const char *set_name, const char *sets[]);

int get_set_index(const char *set_name, const char *sets[])
{
    int i;
    printf("set_name: %s\n", set_name);
    for (i = 0; i < SETS_NUMBER; i++)
    {
        printf("sets[%d]: %s\n", i, sets[i]);
        if (strcmp(set_name, sets[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int parse_command(char *input, set *sets)
{
    const char *set_names[SETS_NUMBER] = {"SETA", "SETB", "SETC", "SETD", "SETE", "SETF"};
    char command[20];
    char args[1000];
    char *token;
    token= NULL;
    printf("input: %s\n", input);
    if (sscanf(input, "%s %[^\n]", command, args) < 1)
    {
        return 0;
    }

    if (strcmp(command, "read_set") == 0)
    {
        char setName[10];
        int values[128], count = 0, num;
        int setIndex;
        token = strtok(args, " ,");
        if (token)
        {
            strcpy(setName, token);
            setIndex = get_set_index(setName, set_names);
            if (setIndex == -1)
            {
                printf("Undefined set name\n");
                return 0;
            }
            token = strtok(NULL, ",");
            while (token && sscanf(token, "%d", &num) && num != -1)
            {
                if (num < 0 || num > 127)
                {
                    printf("Invalid set member - value out of range\n");
                    return 0;
                }
                values[count++] = num;
                token = strtok(NULL, ",");
            }
            if (num != -1)
            {
                printf("List of set members is not terminated correctly\n");
                return 0;
            }
            read_set(&sets[setIndex], values, count);
        }
    }
    else if (strcmp(command, "print_set") == 0)
    {
        int setIndex = get_set_index(args, set_names);
        if (setIndex == -1)
        {
            printf("Undefined set name\n");
            return 0;
        }
        print_set(&sets[setIndex]);
    }
    else if (strcmp(command, "union_set") == 0 || strcmp(command, "intersect_set") == 0 ||
             strcmp(command, "sub_set") == 0 || strcmp(command, "symdiff_set") == 0)
    {
        char *setName1;
        char *setName2;
        char *resultName;
        int setIndex1, setIndex2, resultIndex;
        setName1 = strtok(args, ", ");
        setName2 = strtok(NULL, ", ");
        resultName = strtok(NULL, ", ");
        printf("setName1: %s\n", setName1);
        printf("setName2: %s\n", setName2);
        printf("resultName: %s\n", resultName);
        if (!setName1 || !setName2 || !resultName)
        {
            printf("Invalid command format\n");
            return 0;
        }
        setIndex1 = get_set_index(setName1, set_names);
        setIndex2 = get_set_index(setName2, set_names);
        resultIndex = get_set_index(resultName, set_names);
        printf("setIndex1: %d\n", setIndex1);
        printf("setIndex2: %d\n", setIndex2);
        printf("resultIndex: %d\n", resultIndex);
        if (setIndex1 == -1 || setIndex2 == -1 || resultIndex == -1)
        {
            printf("Undefined set name\n");
            return 0;
        }
        if (strcmp(command, "union_set") == 0)
        {
            union_set(&sets[resultIndex], &sets[setIndex1], &sets[setIndex2]);
        }
        else if (strcmp(command, "intersect_set") == 0)
        {
            intersect_set(&sets[resultIndex], &sets[setIndex1], &sets[setIndex2]);
        }
        else if (strcmp(command, "sub_set") == 0)
        {
            subtract_set(&sets[resultIndex], &sets[setIndex1], &sets[setIndex2]);
        }
        else if (strcmp(command, "symdiff_set") == 0)
        {
            symdiff_set(&sets[resultIndex], &sets[setIndex1], &sets[setIndex2]);
        }
    }
    else if (strcmp(command, "stop") == 0)
    {
        return -1;
    }
    else
    {
        printf("Undefined command name\n");
    }
    return 0;
}

int main(void)
{
    set sets[SETS_NUMBER];
    char input[1024];
    init_sets(sets, SETS_NUMBER);

    while (1)
    {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Error reading input\n");
            continue;
        }
        if (parse_command(input, sets) == -1)
        {
            break;
        }
    }

    return 0;
}
