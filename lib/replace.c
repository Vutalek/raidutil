#include "headers/replace.h"
#include "headers/string.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <regex.h>

char* build_progressbar_string(int width, int percent)
{
    int progress_count = (percent / 100.0) * width;
    char* progressbar = (char*) malloc(2 + width + 1);
    progressbar[0] = '[';
    for(int i = 0; i < progress_count; i++)
        progressbar[i+1] = '#';
    for (int i = 0; i < (width - progress_count); i++)
        progressbar[i+1+progress_count] = ' ';
    progressbar[width] = ']';
    progressbar[width+1] = '\0';
    return progressbar;
}

void replace(char* disk, char* replacer)
{
    char* array = raid_array_of_the_disk(disk);
    char command[100];
    sprintf(command, "sudo mdadm /dev/%s --add %s", array, replacer);
    system(command);
    sprintf(command, "sudo mdadm /dev/%s --replace %s --with %s", array, disk, replacer);
    system(command);

    while(!recovery_finished(array));
    sprintf(command, "sudo mdadm /dev/%s --remove %s", array, disk);
    system(command);
    free(array);
}

char* raid_array_of_the_disk(char* disk)
{
    char* array = 0;
    FILE* mdstat = fopen("/proc/mdstat", "r");
    if (mdstat == NULL)
        return NULL;

    char buffer[256];
    while(fgets(buffer, 256, mdstat) != NULL)
    {
        if (regex_match(buffer, disk+5))
        {
            regex_t regex;
            regmatch_t pmatch[1];
            int regres = regcomp(&regex, "md[0-9]+", REG_EXTENDED);
            regres = regexec(&regex, buffer, 1, pmatch, 0);
            if(!regres)
            {
                array = (char*) malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
                sprintf(array, "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, buffer);
            }
            regfree(&regex);
            break;
        }
    }

    fclose(mdstat);
    return array;
}

bool recovery_finished(char* array)
{
    bool finished = false;
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
        regex_t regex;
        regmatch_t pmatch[1];
        int regres = regcomp(&regex, "[0-9]+[.][0-9]+%", REG_EXTENDED);
        regres = regexec(&regex, buffer, 1, pmatch, 0);
        if (!regres)
        {
            char* percent_string = (char*) malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
            sprintf(percent_string, "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, (buffer + pmatch[0].rm_so));
            char* progressbar = build_progressbar_string(50, atoi(percent_string));
            printf("\rRecovery in process %s %s", progressbar, percent_string);
            fflush(stdout);
            free(progressbar);
            free(percent_string);
        }
        regfree(&regex);
    }
    else
    {
        printf("\nRecovery finished.\n");
        finished = true;
    }
    fclose(mdstat);
    return finished;
}
