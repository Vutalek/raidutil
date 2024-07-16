#ifndef REPLACE_H
#define REPLACE_H

#include <stdbool.h>

void replace(char* disk, char* replacer);

char* raid_array_of_the_disk(char* disk);

bool recovery_finished(char* array);

char* build_progressbar_string(int width, int percent);

#endif
