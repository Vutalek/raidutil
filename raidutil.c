#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <stdlib.h>

void copy_to_cmd_if_found(char*, char*);

int main(int argc, char* argv[])
{
    if (argc == 1)
        printf("No work to do.\n");
    else if (!strcmp(argv[1], "status"))
    {
        char buffer[256];
        char** raid_arrays;
        regex_t regex;
        regmatch_t pmatch[1];
        int a = regcomp(&regex, "md[0-9]+", REG_EXTENDED);
        FILE* mdstat = fopen("/proc/mdstat", "r");
        if (mdstat == NULL)
        {
            printf("Doesn't have any RAID\n");
            return 0;
        }
        int i = 0;
        while(fgets(buffer, 256, mdstat) != NULL)
        {
            a = regexec(&regex, buffer, 1, pmatch, 0);
            if(!a)
            {
                raid_arrays = (char**) realloc(raid_arrays, sizeof(char*) * (i+1));
                raid_arrays[i] = (char*) malloc(pmatch[0].rm_eo - pmatch[0].rm_so);
                sprintf(raid_arrays[i], "%.*s", pmatch[0].rm_eo - pmatch[0].rm_so, buffer);
                i++;
            }
        }
        regfree(&regex);
        fclose(mdstat);
        int n = i;
        printf("Found %d RAID-arrays:\n", i);
        if (i == 0)
            printf("No work to do.");
        else
        {
            for(int i = 0; i < n; i++)
                printf("/dev/%s\n", raid_arrays[i]);
            printf("\nInformation on each RAID array.\n\n");
            for(int i = 0; i < n; i++)
            {
                char command[100];
                sprintf(command, "sudo mdadm --detail /dev/%s", raid_arrays[i]);
                FILE* detail_info = popen(command, "r");
                fgets(buffer, 256, detail_info);
                printf(buffer);
                char** disks;
                int k = 0;
                while(fgets(buffer, 256, detail_info) != NULL)
                {
                    copy_to_cmd_if_found(buffer, "Persistence");
                    copy_to_cmd_if_found(buffer, "State : ");
                    copy_to_cmd_if_found(buffer, "Active Devices");
                    copy_to_cmd_if_found(buffer, "Working Devices");
                    copy_to_cmd_if_found(buffer, "Failed Devices");
                    copy_to_cmd_if_found(buffer, "Spare Devices");
                    regcomp(&regex, "/dev/sd", REG_EXTENDED);
                    a = regexec(&regex, buffer, 0, NULL, 0);
                    if (!a)
                    {
                        disks = (char**) realloc(disks, sizeof(char*) * (k+1));
                        disks[k] = (char*) malloc(256);
                        disks[k] = memcpy(disks[k], buffer, 256);
                        k++;
                    }
                }
                regfree(&regex);

                //mdadm --examine /dev/sdX

                printf("Disks status:\n");
                regcomp(&regex, "[^ \f\n\r\t\v]+", REG_EXTENDED);
                regmatch_t temp[1];
                char** disk_pmatch;
                char* subbuffer = disks[0];
                for(int j = 0; j < 7; j++)
                {
                    regexec(&regex, subbuffer, 1, temp, 0);
                    disk_pmatch = (char**) realloc(disk_pmatch, sizeof(char*) * (j+1));
                    disk_pmatch[j] = (char*) malloc(temp[0].rm_eo - temp[0].rm_so);
                    sprintf("%.*s", temp[0].rm_eo - temp[0].rm_so, subbuffer + temp[0].rm_so);
                    subbuffer += temp[0].rm_eo;
                }
                printf(disk_pmatch[3]);
                printf(disk_pmatch[4]);
                printf(disk_pmatch[6]);

                regfree(&regex);

                pclose(detail_info);
                for(int j = 0; j < k; j++)
                    free(disks[j]);
                free(disks);
            }
        }
        for(int i = 0; i < n; i++)
            free(raid_arrays[i]);
        free(raid_arrays);
    }
    else
        printf("Unknown command\n");
    return 0;
}

void copy_to_cmd_if_found(char* buffer, char* need_to_found)
{
    regex_t regex;
    int a = regcomp(&regex, need_to_found, 0);
    a = regexec(&regex, buffer, 0, NULL, 0);
    if (!a) printf(buffer);
    regfree(&regex);
}
