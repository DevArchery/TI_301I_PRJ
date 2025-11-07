#include <stdio.h>
#include "data.h"
#include "utils.h"
#include <stdlib.h>
int main() {
    t_adjList* graph=readGraph("../data/exemple_valid_step3.txt");
    displayAdjList(graph);
    checkMarkovValidity(graph);
    mermaidOutput(graph);
    return 0;
}
