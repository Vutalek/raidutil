#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

char** split(char* str, char* delimeters, int* array_counter);

char* trim(char* str);

char* copy(char* str);

#endif
