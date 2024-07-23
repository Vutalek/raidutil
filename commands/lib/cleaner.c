#include "headers/cleaner.h"

#include <stdlib.h>

//cleaning 2d-arrays or jagged arrays
void clean2d(void*** arr, int rows)
{
    for(int i = 0; i < rows; i++)
        free((*arr)[i]);
    free(*arr);
}
