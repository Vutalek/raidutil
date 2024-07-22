#include "headers/get.h"

#include "lib/headers/regex.h"
#include "lib/headers/cmdoutput.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//main function of this command
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
    else printf("This disk is not in RAID.\n");
}

//checks if a disk has RAID metadata
bool disk_in_raid_array(char* disk)
{
    char command[100];
    sprintf(command, "sudo mdadm --examine %s &> /dev/null", disk);
    int result = system(command);
    if (result)
        return false;
    else return true;
}
