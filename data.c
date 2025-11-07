#include <stdlib.h>
#include <stdio.h>
#include "data.h"
t_adjList* readGraph(char *filename) {
    /*
    IN: String that is the fileneame
    OUT: Pointer to adjacency list
    Usage: Converts a text file to an adjacency list that is usable for other functions
     */
    t_adjList* adjList = malloc(sizeof(t_adjList));
    if (!adjList) {
        perror("malloc for adjList structure failed");
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(adjList);
        return NULL;
    }

    char header_line[256];
    if (fgets(header_line, sizeof(header_line), file) == NULL || sscanf(header_line, "%d", &adjList->size) != 1) {
        fprintf(stderr, "Error: Malformed or missing header line (expected node count).\n");
        fclose(file);
        free(adjList);
        return NULL;
    }

    adjList->array = malloc(adjList->size * sizeof(t_list*));
    if (!adjList->array) {
        perror("malloc for t_list pointer array failed");
        fclose(file);
        free(adjList);
        return NULL;
    }

    for (int i = 0; i < adjList->size; i++) {
        adjList->array[i] = malloc(sizeof(t_list));
        if (!adjList->array[i]) {
            perror("malloc for individual t_list failed");
            // Proper cleanup would be needed here in a real application
            fclose(file);
            free(adjList->array);
            free(adjList);
            return NULL;
        }
        adjList->array[i]->head = NULL;
    }

    char line[256];
    int source, dest;
    float probability;

    while (fgets(line, sizeof(line), file) != NULL) {
        int items_scanned = sscanf(line, "%d %d %f", &source, &dest, &probability);

        if (items_scanned == 3) {
            // Validate the 1-based numbers from the file. This can lead to an output still being produced but it won't be the expected one
            if (source < 1 || source > adjList->size || dest < 1 || dest > adjList->size) {
                fprintf(stderr, "Warning: Skipping invalid edge (%d, %d) which is out of bounds for size %d.\n", source, dest, adjList->size);
                continue;
            }

            t_cell* newCell = createCell(dest, probability);
            if (!newCell) {
                // Handle memory allocation failure
                fclose(file);
                return NULL;
            }

            int source_index = source - 1;
            newCell->next = adjList->array[source_index]->head;
            adjList->array[source_index]->head = newCell;

        } else if (items_scanned != EOF && items_scanned > 0) {
            fprintf(stderr, "Warning: Skipping malformed line: %s", line);
        }
    }

    fclose(file);
    return adjList;
}