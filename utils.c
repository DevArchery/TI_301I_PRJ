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
//Structures implementation
typedef struct s_cell {
    int arrival_vertex;
    int probability;
    struct s_cell *next;
}t_cell;

typedef struct s_list {
    t_cell *head;
}t_list;

typedef struct s_adjList {
    t_list *list;
    int size;
}t_adjList;


//Functions


t_cell* createCell(int arrival_vertex, int prob) {
    t_cell * NewCell;
    NewCell->arrival_vertex = arrival_vertex;
    t_cell * next = NULL;
    int next_prob = prob;
    return NewCell;
}

t_list* createEmptyList() {
    t_list * NewList;
    t_cell * head = NULL;
    return NewList;
}

t_list* addCellList(t_list* list, t_cell* cell) {
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
    t_cell* cur = list->head;
    printf("[head @]");
    while (cur != NULL) {
        printf(" @-> (%d, %d)", cur->arrival_vertex, cur->probability);
        cur = cur->next;
    }
    printf("\n");
}



t_list** CreateEmptyAdjencyList(int size){
    /*IN :
     OUT :
     Usage :
     */
    t_list **AdjArray; //we need to use double pointers therefor the first pointer points to the array, which inside has pointers to the list
    AdjArray = (t_list**)malloc(size * sizeof(t_list *));
    for (int i = 0; i < size; i++) {
        AdjArray[i] = createEmptyList();
    }
    return AdjArray; //return the double pointer to the function
}


void displayAdjList(t_list** AdjArray,int size) {
    t_list* cur = AdjArray[0];
    for (int i = 0; i < size; i++) {
        printf("List for vertex %d : ",i);
        displayList(AdjArray[i]);
    }
}
