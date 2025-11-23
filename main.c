#include <stdio.h>
#include "data.h"
#include "utils.h"
#include "hasse.h"
#include <stdlib.h>
int main() {
    printf("Hello this is a graph analysis tool made for the TI301I class.\n");
    printf("This program is made to read, manipulate and interpret data from graphs.\n");
    printf("First, start by loading a graph by giving the path to the .txt file.\n");
    printf("Some graphs are already preloaded in the data folder but feel free to create new ones.\n");
    printf("Here are some specific tests you can try\n ");

    //Tests
    /*
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
    */
    return 0;
}