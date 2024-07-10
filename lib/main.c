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
    {
        char** raid_arrays;
        int number_of_arrays = 0;
        raid_arrays = find_raid_arrays(&number_of_arrays);

        printf("Found %d RAID-arrays:\n", number_of_arrays);
        for(int i = 0; i < number_of_arrays; i++)
            printf("/dev/%s\n", raid_arrays[i]);

        printf("\nInformation on each RAID array.\n\n");
        for(int i = 0; i < number_of_arrays; i++)
            raid_array_status(raid_arrays[i]);

        clean2d((void***)&raid_arrays, number_of_arrays);
    }
    else
        printf("Unknown command\n");
    return 0;
}

