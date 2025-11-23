#ifndef PROJECT_DATA_H
#define PROJECT_DATA_H
#include "utils.h"

t_adjList* readGraph(char *filename);
void TarjanAlgorithm(t_adjList* adjList);

#endif //PROJECT_DATA_H