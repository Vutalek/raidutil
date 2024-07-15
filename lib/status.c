#include "headers/status.h"
#include "headers/string.h"

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

void status()
{
    int number_of_arrays = 0;
    char** raid_arrays = find_raid_arrays(&number_of_arrays);
    if (raid_arrays != NULL)
    {
        printf("Found %d RAID-arrays:\n", number_of_arrays);
        for(int i = 0; i < number_of_arrays; i++)
            printf("/dev/%s\n", raid_arrays[i]);

        printf("\nInformation on each RAID array.\n\n");
        for(int i = 0; i < number_of_arrays; i++)
            raid_array_status(raid_arrays[i]);

        clean2d((void***)&raid_arrays, number_of_arrays);
    }
    else
        printf("There is no RAID.\n");
}

char** find_raid_arrays(int* num)
{
    char** result = 0;
    *num = 0;
    regex_t regex;
    regmatch_t pmatch[1];
    int regres = regcomp(&regex, "md[0-9]+", REG_EXTENDED);
    FILE* mdstat = fopen("/proc/mdstat", "r");
    if (mdstat == NULL)
        return NULL;
    char buffer[256];
    while(fgets(buffer, 256, mdstat) != NULL)
    {
        regres = regexec(&regex, buffer, 1, pmatch, 0);
        if(!regres)
        {
            result = (char**) realloc(result, sizeof(char*) * (*num+1));
            result[*num] = (char*) malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
            sprintf(result[*num], "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, buffer);
            (*num)++;
        }
    }
    regfree(&regex);
    fclose(mdstat);
    return result;
}

void raid_array_status(char* arr)
{
    char command[100];
    char buffer[256];

    sprintf(command, "sudo mdadm --detail /dev/%s", arr);
    FILE* detail_info = popen(command, "r");

    fgets(buffer, 256, detail_info);
    printf(buffer);

    char** disks = 0;
    int number_of_disks = 0;
    regex_t regex;
    int regres = regcomp(&regex, "/dev/sd", 0);
    while(fgets(buffer, 256, detail_info) != NULL)
    {
        trim_and_copy_to_cmd_if_found(buffer, "Persistence");
        trim_and_copy_to_cmd_if_found(buffer, "^[ ]*State");
        trim_and_copy_to_cmd_if_found(buffer, "Active Devices");
        trim_and_copy_to_cmd_if_found(buffer, "Working Devices");
        trim_and_copy_to_cmd_if_found(buffer, "Failed Devices");
        trim_and_copy_to_cmd_if_found(buffer, "Spare Devices");
        regres = regexec(&regex, buffer, 0, NULL, 0);
        if (!regres)
        {
            disks = (char**) realloc(disks, sizeof(char*) * (number_of_disks+1));
            disks[number_of_disks] = (char*) malloc(256);
            disks[number_of_disks] = memcpy(disks[number_of_disks], buffer, 256);
            number_of_disks++;
        }
    }
    regfree(&regex);

    printf("Disks status:\n");
    for(int i = 0; i < number_of_disks; i++)
        disk_from_array_short_status(disks[i]);
    printf("\n");

    clean2d((void***)&disks, number_of_disks);
    pclose(detail_info);
}

void disk_from_array_short_status(char* mdadm_detail_disk_str)
{
    int n = 0;
    char** parsed_str = split(mdadm_detail_disk_str, " \n", &n);
    printf("[%s] %s %s\n", parsed_str[3], parsed_str[n-1], parsed_str[4]);
    clean2d((void***)&parsed_str, n);
}
