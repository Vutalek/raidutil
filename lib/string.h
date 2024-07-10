#ifndef STRING_H
#define STRING_H

char** split(char* str, char* delimeters, int* array_counter);

char* copy(char* str);

void clean2d(void*** arr, int rows);

char* regex_match_copy_full_str(char* str, char* regex_pattern);

#endif
