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
 * @brief represents and adjacency list
 * @param array : pointer to an array of lists, each list contains the successors of a vertex
 * @param size : number of vertices in the graph (size of array)
 * @return : structure used to store the adjacency representation
 */

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
/***
 * @brief represents a cell in Tarjan's adjacency list
 * @param node_index : index of the vertex the points to
 * @param next : pointer to next cell
 * @return : structure used to build adjacency list
 */

typedef struct t_tarjanVertex {
    int identifier;
    int nbr_classes;
    int accessible_number;
    int state;
} t_tarjanVertex;
/***
 * @brief represents a vertex in Tarjan's algorithm
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

t_tarjanVertex* createTarjanVertex(int node_id);

// CLASS
typedef struct t_tarjanNode {
    t_tarjanVertex* vertex;
    struct t_tarjanNode* next;
} t_classNode;
/***
 * @brief
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

typedef struct t_class {
    char name[10];
    t_classNode* head;
    int size;
} t_class;
/***
 * @brief
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

t_class* createClass(const char*);
void addVertexToClass(t_class*, t_tarjanVertex*);
t_tarjanVertex ** tarjanStateArray();

// STACK
typedef struct t_stack {
    t_tarjanVertex** vertex;
    int top;
    int size;
} t_stack;
/***
 * @brief
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

t_stack* createStack(int);
void push(t_stack*, t_tarjanVertex*);
t_tarjanVertex* pop(t_stack*);
int isEmptyStack(t_stack*);

// PARTITION
typedef struct t_partitionNode {
    t_class* class;
    struct t_partitionNode* next;
} t_partitionNode;
/***
 * @brief
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

typedef struct t_partition {
    t_partitionNode* head;
    int size;
} t_partition;
/***
 * @brief
 * @param identifier :
 * @param nbr_classes :
 * @param accessible_number :
 * @param state :
 * @return :
 */

t_partition* createPartition();
void addClassToPartition(t_partition*, t_class*);

#endif