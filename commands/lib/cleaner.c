#include "headers/cleaner.h"

#include <stdlib.h>

void clean2d(void*** arr, int rows)
{
    for(int i = 0; i < rows; i++)
        free((*arr)[i]);
    free(*arr);
}
