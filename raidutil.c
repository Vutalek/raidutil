#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <stdlib.h>

#include "lib/headers/string.h"
#include "lib/headers/status.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
        printf("No work to do.\n");
    else if (!strcmp(argv[1], "status"))
        status();
    else
        printf("Unknown command\n");
    return 0;
}

