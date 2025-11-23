#include <stdio.h>
#include "data.h"
#include "utils.h"
#include "hasse.h"
#include <stdlib.h>
int main() {
    t_adjList* graph=readGraph("../data/exemple_valid_step3.txt");
    displayAdjList(graph);
    checkMarkovValidity(graph);
    mermaidOutput(graph);
    t_partition* part = TarjanAlgorithm(graph);
    if (part != NULL) {
        displayPartition(part);
    }
    t_link_array links = createLinkArray(part, graph);
    removeTransitiveLinks(&links);
    printLinkArray(&links);
    writeHasseMermaid(part, graph, "../hasse_output.txt");
    freeLinkArray(&links);
    return 0;
}