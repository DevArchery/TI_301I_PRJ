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
/***
 * @brief create a zero matrix of size n x n
 * @param columns : number of columns of the matrix
 * @param rows : number of rows of the matrix
 * @return : the created matrix
 */

matrix createZeroMatrix (int n);
/***
 * @brief create a zero matrix of size n x n
 * @param n : size of the matrix
 * @return : the created matrix filled with zeros
 */

matrix adjencyListToMatrix (t_adjList graph);
/***
 * @brief convert an adjacency list to a matrix
 * @param graph : the adjacency list to convert
 * @return : the converted matrix
 */

void copyMatrix (matrix dest, matrix src);
/***
 * @brief copy the content of a matrix to another matrix
 * @param dest : destination matrix
 * @param src : source matrix
 */

matrix matrixToPower (matrix M, float power);
/***
 * @brief raise each probability of a matrix to a given power
 * @param M : the matrix to raise to a power
 * @param power : the power to raise each probability to
 * @return : the resulting matrix
 */

matrix multiplyMatrix (matrix A, matrix B);
/***
 * @brief multiply two matrices
 * @param A : first matrix
 * @param B : second matrix
 * @return : the resulting matrix
 */

float matrixDiff(matrix A, matrix B);
/***
 * @brief calculate the difference between two matrices
 * @param A : first matrix
 * @param B : second matrix
 * @return : the difference between the two matrices
 */



#endif //INC_2526_TI301_PJT_MATRIX_H