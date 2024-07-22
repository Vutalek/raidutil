#ifndef REPLACE_H
#define REPLACE_H

#include <stdbool.h>

void replace(char* disk, char* replacer);

char* raid_array_of_the_disk(char* disk);

void recovery_progress(char* array);

bool recovery_finished(char* array);

#endif
