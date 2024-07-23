#include "headers/cmdoutput.h"

#include "headers/string.h"
#include "headers/regex.h"

#include <stdio.h>
#include <stdlib.h>

//checks if str matches regex pattern need_to_find
//if it is, then str is trimmed and printed in console
void trim_and_copy_to_cmd_if_found(char* str, char* need_to_find)
{
    char* copy_str = regex_match_copy_full_str(str, need_to_find);
    if (copy_str != NULL)
    {
        char* trimmed_str = trim(copy_str);
        printf("%s\n", trimmed_str);
        free(trimmed_str);
    }
    free(copy_str);
}

//this function bulids string of progress bar in such way
//width = 5, percent = 60
//result string:
//[###  ]
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
