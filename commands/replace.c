#include "headers/replace.h"

#include "lib/headers/regex.h"
#include "lib/headers/cmdoutput.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//main function of this command
void replace(char* disk, char* replacer)
{
    char* array = raid_array_of_the_disk(disk);
    if (array == NULL)
    {
        printf("This disk is not in an array.\n");
        return;
    }
    char command[100];

    sprintf(command, "sudo mdadm /dev/%s --add %s", array, replacer);
    int result = system(command);
    if (result)
    {
        printf("Something wrong went, when adding new disk to an array.\n");
        return;
    }

    sprintf(command, "sudo mdadm /dev/%s --replace %s --with %s", array, disk, replacer);
    result = system(command);
    if (result)
    {
        printf("Something wrong went, while replacing disk in the array.\n");
        return;
    }

    while(!recovery_finished(array))
    {
        recovery_progress(array);
        sleep(1);
    }
    printf("\nRecovery finished.\n");

    sprintf(command, "sudo mdadm /dev/%s --remove %s", array, disk);
    result = system(command);
    if (result)
    {
        printf("Something wrong went, while removing old disk in the array.\n");
        return;
    }
    free(array);
}

//finds raid_array which contains this disk
//return device name of RAID array such as md127
//result always must be cleaned with free()
char* raid_array_of_the_disk(char* disk)
{
    char* array = 0;
    FILE* mdstat = fopen("/proc/mdstat", "r");
    if (mdstat == NULL)
        return array;

    char buffer[256];
    while(fgets(buffer, 256, mdstat) != NULL)
    {
        if (regex_match(buffer, disk+5))
        {
            int* off_len = regex_match_offset_length(buffer, "md[0-9]+");
            if(off_len != NULL)
            {
                array = (char*) malloc(off_len[1]);
                sprintf(array, "%.*s", off_len[1], buffer);
            }
            free(off_len);
            break;
        }
    }

    fclose(mdstat);
    return array;
}

//checks progress of recovery and print out progress bar
void recovery_progress(char* array)
{
    FILE* mdstat = fopen("/proc/mdstat", "r");
    char buffer[256];
    while(fgets(buffer, 256, mdstat) != NULL)
    {
        if (regex_match(buffer, array))
            break;
    }
    fgets(buffer, 256, mdstat);
    fgets(buffer, 256, mdstat);
    if (regex_match(buffer, "recovery"))
    {
        int* off_len = regex_match_offset_length(buffer, "[0-9]+[.][0-9]+%");
        if (off_len != NULL)
        {
            char* percent_string = (char*) malloc(off_len[1]);
            sprintf(percent_string, "%.*s", off_len[1], (buffer + off_len[0]));
            char* progressbar = build_progressbar_string(50, atoi(percent_string));
            printf("\rRecovery in progress %s %s", progressbar, percent_string);
            fflush(stdout);
            free(progressbar);
            free(percent_string);
        }
    }
    fclose(mdstat);
}

//checks in /proc/mdstat if recovery of array is finished
bool recovery_finished(char* array)
{
    bool finished = true;
    FILE* mdstat = fopen("/proc/mdstat", "r");
    char buffer[256];
    while(fgets(buffer, 256, mdstat) != NULL)
    {
        if (regex_match(buffer, array))
            break;
    }
    fgets(buffer, 256, mdstat);
    fgets(buffer, 256, mdstat);
    if (regex_match(buffer, "recovery"))
        finished = false;
    else finished = true;
    fclose(mdstat);
    return finished;
}
