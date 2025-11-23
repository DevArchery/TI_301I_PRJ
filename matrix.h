//
// Created by rudol on 23/11/2025.
//
#include "utils.h"

#ifndef INC_2526_TI301_PJT_MATRIX_H
#define INC_2526_TI301_PJT_MATRIX_H

typedef struct {
    int columns;
    int rows;
    float **proba;
} matrix;

matrix createZeroMatrix (int n);



#endif //INC_2526_TI301_PJT_MATRIX_H