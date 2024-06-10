#include <stdio.h>

int main(int argc, char *argv[])
{
    while (--argc > 0)
    {
        handle_mcro(argv[argc]);
    }
}