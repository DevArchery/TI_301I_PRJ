#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}





//Functions


t_cell* createCell(int arrival_vertex, int prob) {
    /*
    IN : An integer arrival_vertex and an integer probability
    OUT : A pointer to the created cell
    Usage : Creates a cell with the given arrival_vertex and probability and returns a pointer to it;
    */
    t_cell * NewCell;
    NewCell = (t_cell*)malloc(sizeof(t_cell));
    NewCell->arrival_vertex = arrival_vertex;
    NewCell->next = NULL;
    NewCell->probability = prob;
    return NewCell;
}



t_list* createEmptyList() {
    /*
    IN : No input parameters
    OUT : A pointer to the created empty list
    Usage : Creates an empty list and returns a pointer to it
    */
    t_list * NewList = malloc(sizeof(t_list));
    NewList->head = NULL;
    return NewList;
}



t_list* addCellList(t_list* list, t_cell* cell) {
    /*
    IN : A pointer to the list and a pointer to the cell to add
    OUT : A pointer to the updated list
    Usage : Adds a cell to the end of the list
    */
    if (list->head == NULL) {
        list->head = cell;
    }
    else
    {
        t_cell* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = cell;
    }
    return list;
}



void displayList(t_list* list) {
    /*
    IN : A pointer to the list to display
    OUT : No output parameters
    Usage : Displays the contents of the list
    */
    t_cell* cur = list->head;
    printf("[head @]");
    while (cur != NULL) {
        printf(" @-> (%d, %d)", cur->arrival_vertex, cur->probability);
        cur = cur->next;
    }
    printf("\n");
}



t_list** createEmptyAdjacencyList(int size) {
    /*
    IN : An integer to specify the size of the adjacency list
    OUT : A pointer to the created adjacency list
    Usage : Creates an empty adjacency list with the given size and returns a pointer to it
     */
    while (size <= 0) {
        printf("Error: Size must be a positive integer.\nPleases enter a valid size : ");
        scanf("%d", &size);
    }
    t_list **AdjArray; //we need to use double pointers therefore the first pointer points to the array, which inside has pointers to the list
    AdjArray = (t_list**)malloc(size * sizeof(t_list *));
    for (int i = 0; i < size; i++) {
        AdjArray[i] = createEmptyList();
    }
    return AdjArray; //return the double pointer to the function
}



void displayAdjList(t_list** AdjArray,int size) {
    /*
    IN : A pointer to the adjacency list and an integer specifying its size
    OUT : No output parameters
    Usage : Displays the contents of the adjacency list
    */
    for (int i = 0; i < size; i++) {
        printf("List for vertex %d : ",i);
        displayList(AdjArray[i]);
    }
}
