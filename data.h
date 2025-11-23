#ifndef PROJECT_DATA_H
#define PROJECT_DATA_H
#include "utils.h"

t_adjList* readGraph(char *filename);
/**
 * @brief Displays the given partition.
 *
 * @param p_partition The partition to display.
 */
void parcours(t_tarjanVertex* v, t_adjList* graph, t_tarjanVertex** vertices, t_stack* s, int* idx, t_partition* part);
void displayPartition(t_partition *p_partition);
t_partition* TarjanAlgorithm(t_adjList* adjList);

#endif //PROJECT_DATA_H