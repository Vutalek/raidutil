#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

char** split(char* str, char* delimeters, int* array_counter);

char* trim(char* str);

char* copy(char* str);

void clean2d(void*** arr, int rows);

bool regex_match(char* str, char* regex_pattern);

char* regex_match_copy_full_str(char* str, char* regex_pattern);

void trim_and_copy_to_cmd_if_found(char* str, char* need_to_find);

#endif
