#include "lib/headers/string.h"
#include "lib/headers/status.h"
#include "lib/headers/get.h"

#include <stdio.h>
#include <string.h>

#include <regex.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void replace(char* disk, char* replacer);
char* raid_array_of_the_disk(char* disk);

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

void replace(char* disk, char* replacer)
{
    char* array = raid_array_of_the_disk(disk);
    char command[100];
    sprintf(command, "sudo mdadm /dev/%s --add %s", array, replacer);
    system(command);
    sprintf(command, "sudo mdadm /dev/%s --replace %s --with %s", array, disk, replacer);
    system(command);
    sprintf(command, "sudo mdadm /dev/%s --remove %s", array, disk);
    while(system(command))
    {
        FILE* mdstat = fopen("/proc/mdstat", "r");
        char buffer[256];
        bool position_of_array = false;
        while(fgets(buffer, 256, mdstat) != NULL)
        {
            if (position_of_array)
            {
                if (regex_match(buffer, "recovery"))
                {
                    regex_t regex;
                    regmatch_t pmatch[1];
                    int regres = regcomp(&regex, "[0-9]+[.][0-9]+%", REG_EXTENDED);
                    regres = regexec(&regex, buffer, 1, pmatch, 0);
                    if (!regres)
                    {
                        char* percent_string = (char*) malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
                        sprintf(percent_string, "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, buffer);
                        int percent = atoi(percent_string);
                        int width = 20;
                        int progress_count = (percent / 100) * 20;
                        char* progressbar = (char*) malloc(width + 1 + (pmatch[0].rm_eo - pmatch[0].rm_so) + 1);
                        for (int i = 0; i < progress_count; i++)
                            progressbar[i] = '#';
                        for (int i = progress_count; i < width; i++)
                            progressbar[i] = ' ';
                        progressbar[width] = ' ';
                        sprintf(progressbar+width, "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, buffer);
                        printf("\r%s", progressbar);
                        fflush(stdout);
                        free(progressbar);
                        free(percent_string);
                    }
                    regfree(&regex);
                }
            }
            else if (regex_match(buffer, array))
                position_of_array = true;
        }
        fclose(mdstat);
    }
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
