#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lib/headers/string.h"
#include "lib/headers/status.h"

void get(char* disk);
char** command_output(char* command, int* number_of_lines);
bool disk_in_raid_array(char* disk);

int main(int argc, char* argv[])
{
    if (argc == 1)
        printf("No work to do.\n");
    else if (!strcmp(argv[1], "status"))
        status();
    else if (!strcmp(argv[1], "get"))
    {
        if (argc == 2)
            printf("Please specify the disk.\n");
        else
            get(argv[2]);
    }
    else
        printf("Unknown command\n");
    return 0;
}

void get(char* disk)
{
    if (disk_in_raid_array(disk))
    {
        char command[100];
        sprintf(command, "sudo mdadm --examine %s", disk);
        int number_of_lines = 0;
        char** examine_disk_output = command_output(command, &number_of_lines);

        for(int i = 0; i < number_of_lines; i++)
        {
            copy_to_cmd_if_found_status_str(examine_disk_output[i], "Array UUID");
            copy_to_cmd_if_found_status_str(examine_disk_output[i], "Avail Dev Size");
            copy_to_cmd_if_found_status_str(examine_disk_output[i], "^[ ]*State");
            copy_to_cmd_if_found_status_str(examine_disk_output[i], "Device UUID");
            copy_to_cmd_if_found_status_str(examine_disk_output[i], "Device Role");
            char* array_state = regex_match_copy_full_str(examine_disk_output[i], "Array State");
            if (array_state != NULL)
            {
                char* temp = regex_match_copy_full_str(array_state, "AA");
                if (temp != NULL)
                {
                    printf("Array State : active\n");
                    free(temp);
                    free(array_state);
                    continue;
                }
                temp = regex_match_copy_full_str(array_state, "..");
                if (temp != NULL)
                {
                    printf("Array State : missing\n");
                    free(temp);
                    free(array_state);
                    continue;
                }
                temp = regex_match_copy_full_str(array_state, "RR");
                if (temp != NULL)
                {
                    printf("Array State : replacing\n");
                    free(temp);
                    free(array_state);
                    continue;
                }
            }
        }

        clean2d((void***)&examine_disk_output, number_of_lines);
    }
    else
        printf("This disk is not in RAID.\n");
}

char** command_output(char* command, int* number_of_lines)
{
    char buffer[256];
    *number_of_lines = 0;
    char** output = 0;

    FILE* examine_info = popen(command, "r");
    while (fgets(buffer, 256, examine_info) != NULL)
    {
        output = (char**) realloc(output, sizeof(char*) * (*number_of_lines+1));
        output[*number_of_lines] = (char*) malloc(256);
        output[*number_of_lines] = memcpy(output[*number_of_lines], buffer, 256);
        (*number_of_lines)++;
    }
    pclose(examine_info);

    return output;
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
