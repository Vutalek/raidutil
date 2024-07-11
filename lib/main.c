#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <stdlib.h>

#include "string.h"
#include "status.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
        printf("No work to do.\n");
    else if (!strcmp(argv[1], "status"))
        status();
    else if (!strcmp(argv[1], "test"))
    {
        char* ff = "         asdasd a                \n\n\n";
        printf(ff);
        char* fft = trim(ff);
        printf(fft);
        free(fft);
    }
    else
        printf("Unknown command\n");
    return 0;
}

