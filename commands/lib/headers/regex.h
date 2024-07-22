#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

int* regex_match_offset_length(char* str, char* regex_pattern);

bool regex_match(char* str, char* regex_pattern);

char* regex_match_copy_full_str(char* str, char* regex_pattern);

#endif
