#include "headers/regex.h"

#include "headers/string.h"

#include <stdbool.h>
#include <regex.h>
#include <stdlib.h>

//searches for match in str with regex_pattern
//returns offset in string and length of match
//result always must be cleaned with free()
int* regex_match_offset_length(char* str, char* regex_pattern)
{
    regex_t regex;
    regmatch_t pmatch[1];
    int regres = regcomp(&regex, regex_pattern, REG_EXTENDED);
    regres = regexec(&regex, str, 1, pmatch, 0);
    int* result = 0;
    if (!regres)
    {
        result = (int*) malloc(sizeof(int) * 2);
        //offset
        result[0] = pmatch[0].rm_so;
        //length
        result[1] = pmatch[0].rm_eo - pmatch[0].rm_so;
    }
    return result;
}

//basic bool matching function
bool regex_match(char* str, char* regex_pattern)
{
    regex_t regex;
    int regresult = regcomp(&regex, regex_pattern, REG_EXTENDED);
    if (regresult)
        return false;
    regresult = regexec(&regex, str, 0, NULL, 0);
    if (!regresult)
        return true;
    else
        return false;
}

//returns copy of str if it matches regex_patern
//result always must be cleaned with free()
char* regex_match_copy_full_str(char* str, char* regex_pattern)
{
    if (regex_match(str, regex_pattern))
        return copy(str);
    else
        return NULL;
}
