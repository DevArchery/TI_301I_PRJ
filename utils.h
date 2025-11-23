#ifndef __UTILS_H__
#define __UTILS_H__
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
 * @param probability : probability of the cell
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

typedef struct t_adjList {
    t_list**array;
    int size;
}t_adjList;

t_list* createEmptyList(void);
t_list* addCellList(t_list*, t_cell*);
void displayList(t_list*);



t_list** createEmptyAdjacencyList(int);
void displayAdjList(t_adjList*);
void checkMarkovValidity(t_adjList*);

void mermaidOutput(t_adjList*);

typedef struct t_TarjanCell {
    int node_index;
    struct t_TarjanCell* next;
} t_TarjanCell;

typedef struct t_tarjanVertex {
    int identifier;
    int nbr_classes;
    int accessible_number;
    int state;
} t_tarjanVertex;

t_tarjanVertex* createTarjanVertex(int node_id);

typedef struct t_class {
    char name;

};

#endif