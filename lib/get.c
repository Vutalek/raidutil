#include "headers/get.h"
#include "headers/string.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void get(char* disk)
{
    if (disk_in_raid_array(disk))
    {
        char buffer[256];
        char command[100];
        sprintf(command, "sudo mdadm --examine %s", disk);
        FILE* examine_info = popen(command, "r");
        while (fgets(buffer, 256, examine_info) != NULL)
        {
            trim_and_copy_to_cmd_if_found(buffer, "Array UUID");
            trim_and_copy_to_cmd_if_found(buffer, "Avail Dev Size");
            trim_and_copy_to_cmd_if_found(buffer, "^[ ]*State");
            trim_and_copy_to_cmd_if_found(buffer, "Device UUID");
            trim_and_copy_to_cmd_if_found(buffer, "Device Role");
            if (regex_match(buffer, "Array State"))
            {
                if (regex_match(buffer, "AA"))
                    printf("Array State : active\n");
                else if (regex_match(buffer, ".."))
                    printf("Array State : missing\n");
                else if (regex_match(buffer, "RR"))
                    printf("Array State : replacing\n");
            }
        }
        pclose(examine_info);
    }
    else
        printf("This disk is not in RAID.\n");
}

bool disk_in_raid_array(char* disk)
{
    char buffer[256];
    char command[100];
    sprintf(command, "sudo mdadm --examine %s", disk);
    FILE* examine_info = popen(command, "r");
    char* temp_copy = 0;
    while (fgets(buffer, 256, examine_info) != NULL)
    {
        temp_copy = regex_match_copy_full_str(buffer, "Raid");
        if (temp_copy != NULL)
        {
            free(temp_copy);
            return true;
        }
    }
    pclose(examine_info);
    return false;
}
