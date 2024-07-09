#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char** split(char*, char*, int*);
char* copy(char*);
void clean2d(void***, int);
char* regex_match_copy_full_str(char*, char*);

int main(void)
{
    char* a = "   sadfljkhsdk ksjdfh gkjs     sdkfjhg s sdf kjhkjh  ";
    char* b = regex_match_copy_full_str(a, "sad");
    if (b != NULL)
        printf("%s\n", b);
    else
        printf("1\n");
    return 0;
}

char** split(char* str, char* delimeters, int* array_counter)
{
    char** parsed_array;
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
