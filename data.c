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

void displayPartition(t_partition *p_partition)
{
    if (p_partition == NULL) {
        printf("Partition is NULL\n");
        return;
    }

    t_partitionNode *pnode = p_partition->head;
    while (pnode != NULL) {
        t_class *c = pnode->class;
        if (c == NULL || c->head == NULL) {
            /* If class has no name or vertices, still print its name if available */
            if (c && c->name[0] != '\0') {
                printf("%s {}\n", c->name);
            } else {
                printf("C {}\n");
            }
            pnode = pnode->next;
            continue;
        }

        /* Print class name then the set of vertex identifiers */
        printf("%s {", c->name);
        t_classNode *cn = c->head;
        int first = 1;
        while (cn != NULL) {
            if (cn->vertex != NULL) {
                if (!first) printf(",");
                printf("%d", cn->vertex->identifier);
                first = 0;
            } else {
                if (!first) printf(",");
                printf("?");
                first = 0;
            }
            cn = cn->next;
        }
        printf("}\n");

        pnode = pnode->next;
    }
}

t_partition* TarjanAlgorithm(t_adjList* adjList) {
    if (adjList == NULL) return NULL;

    int n = adjList->size;
    t_tarjanVertex** vertices = tarjanStateArray(adjList);
    t_stack* s = createStack(n);
    t_partition* p = createPartition();
    int idx = 0;

    for (int i = 0; i < n; i++) {
        if (vertices[i]->nbr_classes == -1) {
            parcours(vertices[i], adjList, vertices, s, &idx, p);
        }
    }

    return p;
}