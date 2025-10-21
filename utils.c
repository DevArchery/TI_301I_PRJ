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
    return NewList}

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


