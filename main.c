#include <stdio.h>
#include "data.h"
#include "utils.h"
#include <stdlib.h>
int main() {
    t_adjList* graph=readGraph("../data/exemple_valid_step3.txt");
    displayAdjList(graph);
    checkMarkovValidity(graph);
    mermaidOutput(graph);
    t_class *testVertex=NULL;
    for (int i = 0; i < graph->size; i++) {
        /* pass 1-based node and NULL so createClass returns a fresh class */
        t_class *testVertex = createClass(i + 1, NULL, graph);
        displayClassConnected(testVertex);

        /* free the created class to avoid leaks */
        if (testVertex) {
            free(testVertex->connected);
            free(testVertex);
        }
    }
}