#include "headers/string.h"

#include <stdlib.h>
#include <string.h>
#include <regex.h>

char** split(char* str, char* delimeters, int* array_counter)
{
    char** parsed_array = 0;
    *array_counter = 0;
    char* copied_str = copy(str);
    char* token = strtok(copied_str, delimeters);
    int n = 0;
    while(token != NULL)
    {
        parsed_array = (char**) realloc(parsed_array, sizeof(char*) * (n+1));
        parsed_array[n] = copy(token);
        (*array_counter)++;
        n++;
        token = strtok(NULL, delimeters);
    }
    free(copied_str);
    return parsed_array;
}

char* trim(char* str)
{
    int begin = 0;
    int end = strlen(str);

    for (int i = 0; i < (strlen(str)+1); i++)
    {
        if ((str[i] != ' ') && (str[i] != '\n'))
        {
            begin = i;
            break;
        }
    }
    for (int i = strlen(str)-1; i > 0; i--)
    {
        if ((str[i] != ' ') && (str[i] != '\n'))
        {
            end = i;
            break;
        }
    }

    char* trimmed_str = (char*) malloc(end - begin + 2);
    trimmed_str = memcpy(trimmed_str, str+begin, (end - begin + 2));
    trimmed_str[end - begin + 1] = '\0';
    return trimmed_str;
}

char* copy(char* str)
{
    char* copied_str = (char*) malloc(strlen(str) + 1);
    copied_str = memcpy(copied_str, str, (strlen(str) + 1));
    return copied_str;
}

void clean2d(void*** arr, int rows)
{
    for(int i = 0; i < rows; i++)
        free((*arr)[i]);
    free(*arr);
}

char* regex_match_copy_full_str(char* str, char* regex_pattern)
{
    regex_t regex;
    int regresult = regcomp(&regex, regex_pattern, REG_EXTENDED);
    if (regresult)
        return NULL;
    regresult = regexec(&regex, str, 0, NULL, 0);
    if (!regresult)
        return copy(str);
    else
        return NULL;
}
