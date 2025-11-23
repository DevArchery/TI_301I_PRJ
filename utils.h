#ifndef __UTILS_H__
#define __UTILS_H__
#include <string.h>
// Function prototypes from utils.c
char *getID(int);


typedef struct t_cell {
    int arrival_vertex;
    float probability;
    struct t_cell *next;
} t_cell;
/***
 * @brief create a cell
 * @param arrival_vertex : vertex where the cell arrives
 * @param prob : probability of the cell
 * @return : pointer to the created cell
 */

t_cell* createCell(int, float);



typedef struct t_list {
    t_cell *head;
} t_list;
/***
 * @brief create an empty list
 * @return : pointer to the created empty list
 */

typedef struct s_adjList {
    t_list**array;
    int size;
}t_adjList;
/***
 * @brief struct to represent an adjacency list
 * @return Double pointer and size
 */


typedef struct s_class {
    int* connected;
    int size;
    int nbelts;
}t_class;

//List methods
t_list* createEmptyList();
t_list* addCellList(t_list*, t_cell*);
void displayList(t_list*);


//Adjacency list methods
t_list** createEmptyAdjacencyList(int);
void displayAdjList(t_adjList*);

//Vertex methods
t_class* createClass(int, t_class* ,t_adjList*);
void displayClassConnected(t_class*);
//
void checkMarkovValidity(t_adjList*);

void mermaidOutput(t_adjList*);

#endif