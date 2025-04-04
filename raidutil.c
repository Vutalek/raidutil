#include "commands/headers/status.h"
#include "commands/headers/get.h"
#include "commands/headers/replace.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc == 1)
        printf("No work to do.\n");
    else if (!strcmp(argv[1], "status"))
        status();
    else if (!strcmp(argv[1], "get"))
    {
        if (argc <= 2)
            printf("Please specify the disk.\n");
        else
            get(argv[2]);
    }
    else if (!strcmp(argv[1], "replace"))
    {
        if (argc <= 3)
            printf("Please specify the disk.\n");
        else
            replace(argv[2], argv[3]);
    }
    else
        printf("Unknown command\n");
    return 0;
}
