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
 * @param identifier : a unique identifier of the vertex
 * @param nbr_classes : an integer representing the order in which the vertex was visited
 * @param accessible_number : an integer representing the lowest reachable nbr_classes from this vertex
 * @param state : an integer representing if the vertex is in the stack (1) or not (0)
 * @return : structure used to store Tarjan vertex information
 */

t_tarjanVertex* createTarjanVertex(int node_id);

// CLASS
typedef struct t_tarjanNode {
    t_tarjanVertex* vertex;
    struct t_tarjanNode* next;
} t_classNode;
/***
 * @brief  represents a node in a class
 * @param  vertex : A pointer to a Tarjan vertex
 * @param  next : A pointer to the next class node
 * @return : A pointer to the created class node
 */

typedef struct t_class {
    char name[10];
    t_classNode* head;
    int size;
} t_class;
/***
 * @brief  : represents a class of strongly connected components
 * @param  name : The name of the class
 * @param  head : A pointer to the head of the class node list
 * @param  size : The number of nodes in the class
 * @return : A pointer to the created class
 */

t_class* createClass(const char*);
/***
 * @brief  : creates a new class with the given name
 * @param  name : The name of the class
 * @param  head : A pointer to the head of the class node list
 * @param  size : The number of nodes in the class
 * @return : A pointer to the created class
 */

void addVertexToClass(t_class*, t_tarjanVertex*);
/***
 * @brief  : adds a Tarjan vertex to the given class
 * @param  c : A pointer to the class
 * @param  v : A pointer to the Tarjan vertex to add
 * @return : void
 */

t_tarjanVertex ** tarjanStateArray();
/***
 * @brief  : creates an array of Tarjan vertices initialized with default values
 * @param  adjList : A pointer to the adjacency list
 * @return : A pointer to the created array of Tarjan vertices
 */

// STACK
typedef struct t_stack {
    t_tarjanVertex** vertex;
    int top;
    int size;
} t_stack;
/***
 * @brief  : represents a stack used in Tarjan's algorithm
 * @param  vertex : An array of pointers to Tarjan vertices
 * @param  top : The index of the top element in the stack
 * @param  size : The maximum size of the stack
 * @return : A pointer to the created stack
 */

t_stack* createStack(int);
/***
 * @brief  : creates a stack with the given size
 * @param  size : The maximum size of the stack
 * @return : A pointer to the created stack
 */


void push(t_stack*, t_tarjanVertex*);
/***
 * @brief  : pushes a Tarjan vertex onto the stack
 * @param  stack : A pointer to the stack
 * @param  vertex : A pointer to the Tarjan vertex to push
 * @return : void
 */

t_tarjanVertex* pop(t_stack*);
/***
 * @brief  : pops a Tarjan vertex from the stack
 * @param  stack : A pointer to the stack
 * @return : A pointer to the popped Tarjan vertex or NULL if stack is empty
 */

int isEmptyStack(t_stack*);
/***
 * @brief  : checks if the stack is empty
 * @param  stack : A pointer to the stack
 * @return : 1 if the stack is empty, 0 otherwise
 */


// PARTITION
typedef struct t_partitionNode {
    t_class* class;
    struct t_partitionNode* next;
} t_partitionNode;
/**
 * @brief  : represents a node in a partition
 * @param  class : A pointer to a class
 * @param  next : A pointer to the next partition node
 * @return : A pointer to the created partition node
 */

typedef struct t_partition {
    t_partitionNode* head;
    int size;
} t_partition;
/***
 * @brief  : represents a partition of classes
 * @param  head : A pointer to the head of the partition node list
 * @param  size : The number of classes in the partition
 * @return : A pointer to the created partition
 */

t_partition* createPartition();
/***
 * @brief  : creates an empty partition
 * @return : A pointer to the created partition
 */

void addClassToPartition(t_partition*, t_class*);
/***
 * @brief  : adds a class to the given partition
 * @param  p : A pointer to the partition
 * @param  c : A pointer to the class to add
 * @return : void
 */

#endif