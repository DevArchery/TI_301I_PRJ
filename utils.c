#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


char *getID(int i)
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
t_cell* createCell(int arrival_vertex, float prob) {
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
    t_list **adjList; //we need to use double pointers therefore the first pointer points to the array, which inside has pointers to the list
    adjList = (t_list**)malloc(size * sizeof(t_list *));
    for (int i = 0; i < size; i++) {
        adjList[i] = createEmptyList();
    }
    return adjList; //return the double pointer to the function
}



void displayAdjList(t_adjList* adjList) {
    /*
    IN: Pointer to adjacency list
    OUT: void
    Usage: display the adjacency list if it is not NULL
     */
    if (adjList == NULL) {
        printf("Error: Adjacency list is NULL\n");
        return;
    }

    int size = adjList->size;
    for (int i = 0; i < size; i++) {
        printf("List for vertex %d: [head @]", i + 1);

        t_list *lst = adjList->array[i];

        if (lst == NULL || lst->head == NULL) {
            printf("\n"); // If empty, print a newline
            continue;
        }

        t_cell *cur = lst->head;
        const char* separator = " -> ";

        while (cur != NULL) {
            printf("%s(%d, %.2f)", separator, cur->arrival_vertex, cur->probability);
            separator = " @-> ";
            cur = cur->next;
        }
        printf("\n");
    }
}
void checkMarkovValidity(t_adjList* adjList) {
    /*
    IN:Pointer to adjacency list
    OUT: void
    Usage: Display if the entered graph is a Markov Chain
     */


    //Check for NULL ptr
    if (adjList == NULL) {
        printf("Error: Cannot check validity of a NULL adjacency list.\n");
        return;
    }

    printf("\n--- Checking Markov Chain Validity ---\n");
    int is_overall_valid = 1; // Flag to track the validity of the graph

    // Iterate for every vertex
    for (int i = 0; i < adjList->size; i++) {
        float probability_sum = 0.0f;
        t_list* current_list = adjList->array[i];

        // 3. Traverse the linked list for the current vertex
        if (current_list != NULL && current_list->head != NULL) {
            t_cell* current_cell = current_list->head;
            while (current_cell != NULL) {
                probability_sum += current_cell->probability;
                current_cell = current_cell->next;
            }
        }
        //handling of float inaccuracies
        if (probability_sum >= 0.99f && probability_sum < 1.01f) {
            printf("Vertex %d: OK (sum = %.4f)\n", i + 1, probability_sum);
        } else {
            printf("Vertex %d: INVALID (sum = %.4f, should be ~1.0)\n", i + 1, probability_sum);
            is_overall_valid = 0; // If any vertex is invalid, the whole graph is.
        }
    }
    if (is_overall_valid) {
        printf("The graph is a Markov chain.\n");
    } else {
        printf("The graph is NOT a Markov chain.\n");
    }
}

void mermaidOutput(t_adjList* adjList) {
    /*
    IN: Pointer to adjacency list
    OUT: text file output.txt in the same directory
    usage: Convert an adjacency list to a text format that can be used to draw charts on mermaid
     */


    // Check for NULL
    if (adjList == NULL || adjList->array == NULL) {
        fprintf(stderr, "Error: Cannot generate Mermaid output from a NULL or invalid graph.\n");
        return;
    }

    //Opening output;txt in writing mode
    FILE* file = fopen("../output.txt", "w");
    if (file == NULL) {
        perror("Error: Could not open file for writing Mermaid output");
        return;
    }

    // Configuration header of the file
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "    layout: elk\n");
    fprintf(file, "    theme: neo\n");
    fprintf(file, "    look: neo\n");
    fprintf(file, "---\n\n");

    fprintf(file, "flowchart LR\n");

    // Node declaration
    for (int i = 0; i < adjList->size; i++) {
        int vertex_num = i + 1;
        fprintf(file, "    %s((%d))\n", getID(vertex_num), vertex_num);
    }
    fprintf(file, "\n"); // Add a blank line for readability

    // Writing edges
    for (int i = 0; i < adjList->size; i++) {
        t_list* current_list = adjList->array[i];
        if (current_list != NULL && current_list->head != NULL) {
            char source_label[10];
            strcpy(source_label, getID(i + 1));

            t_cell* current_cell = current_list->head;
            while (current_cell != NULL) {
                const char* dest_label = getID(current_cell->arrival_vertex);
                fprintf(file, "    %s -->|%.2f|%s\n",
                       source_label,
                       current_cell->probability,
                       dest_label);

                current_cell = current_cell->next;
            }
        }
    }

    // closing
    fclose(file);

    // Print a confirmation message to the console
    printf("Mermaid output successfully written to '%s'\n", "../output.txt");
}

// Step 2 - Utility elements (2.1)
t_tarjanVertex ** tarjanStateArray(t_adjList* adjList)
{
    /*
    IN: Pointer to adjacency list
    OUT: Pointer to array of Tarjan vertices
    Usage: Initializes an array of Tarjan vertices with default values
     */

    int n = adjList->size;
    t_tarjanVertex ** t_tarjan_array = malloc(n*sizeof(t_tarjanVertex*));

    for (int i=0; i<n; i++)
    {
        t_tarjan_array[i] = malloc(sizeof(t_tarjanVertex));
        t_tarjan_array[i]->identifier = i+1;
        t_tarjan_array[i]->nbr_classes = -1;
        t_tarjan_array[i]->accessible_number = -1;
        t_tarjan_array[i]->state = 0;
    }
    return t_tarjan_array;
}

// CLASS
t_class* createClass(const char* name)
{
    /*
    IN: String representing the name of the class
    OUT: Pointer to the created class
    Usage: Creates an empty class with the given name, no vertices and size set to 0
     */

    t_class* c = malloc(sizeof(t_class));
    strcpy(c->name, name);
    c->head = NULL;
    c->size = 0;
    return c;
}

void addVertexToClass(t_class* c, t_tarjanVertex* v)
{
    /*
    IN: Pointer to a class and another to a Tarjan vertex
    OUT: None
    Usage: Adds the given vertex to the class by inserting it at the head of the list
     */

    t_classNode* newNode = malloc(sizeof(t_classNode));
    newNode->vertex = v;
    newNode->next = c->head;
    c->head = newNode;
    c->size++;
}

// STACK
t_stack* createStack(int size)
{
    /*
    IN: Integer specifying the max size of the stack
    OUT: Pointer to the created stack
    Usage: Creates an empty stack with a given size
     */

    t_stack* stack = malloc(sizeof(t_stack));
    stack->vertex = malloc((size)*sizeof(int));
    stack->top = -1;
    stack->size = size;
    return stack;
}

void push(t_stack* stack, t_tarjanVertex* v)
{
    /*
    IN: Pointer to a stack and another to a Tarjan vertex
    OUT: None
    Usage: Pushes the given vertex onto the stack (if there is available capacity)
     */

    if (stack->top < stack->size - 1) {
        stack->top = stack->top+1;
        stack->vertex[stack->top] = v;
    }
}

t_tarjanVertex* pop(t_stack* stack) {
    /*
    IN: Pointer to a stack
    OUT: Pointer to the popped Tarjan vertex or NULL if stack empty
    Usage: Removes and returns the top element of the stack
     */

    if (stack->top >= 0) {
        return stack->vertex[stack->top--];
    }
    return NULL;
}

int isEmptyStack(t_stack* stack) {
    /*
    IN: Pointer to a stack
    OUT: Integer (1 if empty, 0 otherwise)
    Usage: Checks if stack empty
     */

    return stack->top == -1;
}

// PARTITION
t_partition* createPartition()
{
    /*
    IN: None
    OUT: Pointer to the created partition
    Usage: Creates an empty partition with no classes
     */

    t_partition* p = malloc(sizeof(t_partition));
    p->head = NULL;
    p->size = 0;
    return p;
}

void addClassToPartition(t_partition* p, t_class* c)
{
    /*
    IN: Pointer to a partition and another to a class
    OUT: None
    Usage: Adds the given class to the partition by inserting it at the end of the list
     */

    t_partitionNode* newNode = malloc(sizeof(t_classNode));
    newNode->class = c;
    newNode->next = p->head;
    p->head = newNode;
    p->size++;
}


// Step 3 - Breaking down into functions (3.1)
void parcours(t_tarjanVertex* v, t_adjList* graph, t_tarjanVertex** vertices, t_stack* s, int* idx, t_partition* part)
{
    /*
    IN: Vertex, adjacency list, array of vertices, stack, pointer to idx counter, partition
    OUT: None
    Usage: Performs Tarjan's DFS traversal from the given vertex, updates indices
    and builds strongly connected components into partition
     */

    v->nbr_classes = *idx;
    v->accessible_number = *idx;
    *idx += 1;
    push(s, v);
    v->state = 1;

    t_list* successors = graph->array[v->identifier-1];
    t_cell* cur = successors->head;

    while (cur != NULL) {
        t_tarjanVertex* w = vertices[cur->arrival_vertex-1];

        if (w->nbr_classes == -1) {
            parcours(w, graph, vertices, s, idx, part);
            v->accessible_number = (v->accessible_number < w->accessible_number) ? v->accessible_number : w->accessible_number;
        } else if (w->state) {
            v->accessible_number = (v->accessible_number < w->nbr_classes) ? v->accessible_number : w->nbr_classes;
        }
        cur = cur->next;
    }

    if (v->accessible_number == v->nbr_classes) {
        char cname[10];
        sprintf(cname,"C%d", part->size+1);
        t_class* newClass = createClass(cname);
        t_tarjanVertex* w;

        do {
            w = pop(s);
            w->state = 0;
            addVertexToClass(newClass, w);
        } while (w != v);

        addClassToPartition(part, newClass);
    }
}