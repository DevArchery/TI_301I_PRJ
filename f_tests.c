#include <stdlib.h>
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "utils.c"


t_cell** createCellArray(int size) {
    t_cell** cellArray;

    // Allocate array of pointers to t_cell
    cellArray = (t_cell**)malloc(size * sizeof(t_cell*));

    if (cellArray == NULL) {
        return NULL;
    }

    // Initialize each pointer to NULL (empty slots)
    for (int i = 0; i < size; i++) {
        cellArray[i] = NULL;
    }

    return cellArray;
}