//
// Created by rudolf on 23/11/2025.
//

#include "matrix.h"

#include <math.h>
#include <stdlib.h>

matrix createZeroMatrix (int n) {
    matrix NullMatrix;
    NullMatrix.rows = n;
    NullMatrix.columns = n;
    NullMatrix.proba = malloc (n * sizeof (float*));
    for (int i = 0; i<n; i++) {
        NullMatrix.proba[i] = calloc (n, sizeof (float));
    }
    return NullMatrix;
}

matrix adjencyListToMatrix (t_adjList* graph) {
    matrix adjM = createZeroMatrix (graph->size);
    for (int i=0; i<graph->size; i++) {
        t_cell* cur = graph->array[i]->head;
        while (cur != NULL) {
            adjM.proba[i][cur->arrival_vertex] = cur -> probability;
            cur = cur->next;
        }
    }
    return adjM;
}

void copyMatrix (matrix dest, matrix src) {
    for (int i=0; i< src.rows; i++) {
        for (int j=0; j<src.columns; j++) {
            dest.proba[i][j] = src.proba[i][j];
        }
    }
}

matrix matrixToPower (matrix M, float power) {
    int n = M.rows;
    matrix result = createZeroMatrix (n);
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++) {
            result.proba[i][j] = powf (M.proba[i][j],power);
        }
    }
    return result;
}

matrix multiplyMatrix (matrix A, matrix B) {
    int n = A.rows;
    matrix result = createZeroMatrix (n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            float sum = 0;
            for (int k=0; k<n; k++) {
                sum += A.proba[i][k] * B.proba[k][j];
            }
            result.proba[i][j] = sum;
        }
    }
    return result;
}

float matrixDiff(matrix A, matrix B) {
    float diff = 0;
    for (int i=0; i<A.rows; i++) {
        for (int j=0; j<A.columns; j++) {
            diff += fabsf(A.proba[i][j] - B.proba[i][j]);
        }
    }
    return diff;
}

matrix subMatrix (matrix M, t_partition part, int compo_index) {
    t_classNode* tmp = part.head[compo_index].class->head;
    int count = 0;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    int *vertices = malloc(count * sizeof(int));
    tmp = part.head[compo_index].class->head;
    int indx = 0;
    while (tmp != NULL) {
        vertices[indx++] = tmp->vertex->identifier;
        tmp = tmp->next;
    }
    matrix S = createZeroMatrix (count);
    for (int i=0; i<count; i++) {
        for (int j=0; j<count; j++) {
            S.proba[i][j] = M.proba[vertices[i]][vertices[j]];
        }
    }
    free (vertices);
    return S;



}


