#include "set.h"
#define INITIAL_BUF_SIZE 128

int get_set_index(const char *set_name, const char *sets[]);
int parse_command(char *input, set *sets);
char *read_input(void);

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
    char *token, *check_extra;
    token = NULL;
    printf("input: %s\n", input);
    if (sscanf(input, "%s %[^\n]", command, args) < 2)
    {
        printf("Invalid command format - command name or sets name are missing\n");
        return 0;
    }

    if (strstr(input, ",,"))
    {
        printf("Invalid input - consecutive commas detected\n");
        return 0;
    }

    printf("command: %s\n", command);
    printf("args: %s\n", args);

    if (strcmp(command, "read_set") == 0)
    {
        char setName[10];
        int values[128], count = 0, num;
        char extra;
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
            while (token)
            {

                if (sscanf(token, "%d%c", &num, &extra) == 2)
                {
                    printf("Invalid set member - not an integer or extra characters present\n");
                    return 0;
                }
                else if (sscanf(token, "%d", &num) == 1)
                {
                    if (num != -1 && (num < 0 || num > 127))
                    {
                        printf("Invalid set member - value out of range\n");
                        return 0;
                    }
                    values[count++] = num;
                    if (num == -1)
                    {
                        break;
                    }
                    token = strtok(NULL, ",");
                }
                else
                {
                    printf("Invalid set member - not an integer\n");
                    return 0;
                }
            }
            if (values[count - 1] != -1)
            {
                printf("List of set members is not terminated correctly\n");
                return 0;
            }
            count--;
            check_extra = strtok(NULL, ",");
            if (check_extra)
            {
                printf("Invalid input - extraneous text after command\n");
                return 0;
            }
            read_set(&sets[setIndex], values, count);
        }
    }
    else if (strcmp(command, "print_set") == 0)
    {
        int setIndex;
        token = strtok(args, " ,");
        if (!token)
        {
            printf("Invalid command format - set name required\n");
            return 0;
        }

        check_extra = strtok(NULL, " ,");
        if (check_extra)
        {
            printf("Invalid input - extraneous text after command\n");
            return 0;
        }

        setIndex = get_set_index(token, set_names);
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

        if (!setName1 || !setName2 || !resultName)
        {
            printf("Missing parameter\n");
            return 0;
        }
        printf("setName1: %s\n", setName1);
        printf("setName2: %s\n", setName2);
        printf("resultName: %s\n", resultName);

        setIndex1 = get_set_index(setName1, set_names);
        setIndex2 = get_set_index(setName2, set_names);
        resultIndex = get_set_index(resultName, set_names);
        printf("setIndex1: %d\n", setIndex1);
        printf("setIndex2: %d\n", setIndex2);
        printf("resultIndex: %d\n", resultIndex);
        check_extra = strtok(NULL, ",");
        if (check_extra)
        {
            printf("Invalid input - extraneous text after command\n");
            return 0;
        }
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
    else if (command[strlen(command) - 1] == ',')
    {
        printf("Invalid command format - comma at the end of the command\n");
    }
    else
    {
        printf("Undefined command name\n");
    }
    return 0;
}

char *read_input(void)
{

    char *input = (char *)malloc(INITIAL_BUF_SIZE);
    int current_size = INITIAL_BUF_SIZE;
    int len = 0;
    int ch;

    if (input == NULL)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (len == current_size - 1)
        {
            char *new_input;
            current_size *= 2;
            new_input = (char *)realloc(input, current_size);
            if (new_input == NULL)
            {
                free(input);
                perror("Failed to reallocate memory");
                return NULL;
            }
            input = new_input;
        }
        input[len++] = ch;
    }
    input[len] = '\0';

    if (ch == EOF)
    {
        free(input);
        return NULL;
    }

    return input;
}

int main(void)
{
    set sets[SETS_NUMBER];
    char *input;
    int result;

    init_sets(sets, SETS_NUMBER);

    while (1)
    {
        printf("> ");
        input = read_input();
        if (!input)
        {
            printf("Error: End of file encountered or failed to read input.\n");
            break;
        }

        if (strlen(input) == 0)
        {
            printf("Error: No input entered. Please try again.\n");
            continue;
        }

        result = parse_command(input, sets);
        free(input);

        if (result == -1)
        {
            printf("Exit program...\n");
            break;
        }
    }

    return 0;
}
