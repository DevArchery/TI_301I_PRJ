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
t_class* createEmptyClass() {
    t_class* cls = (t_class*)malloc(sizeof(t_class));
    if (cls == NULL) return NULL;
    cls->connected = (int*)malloc(sizeof(int));
    if (cls->connected == NULL) { free(cls); return NULL; }
    cls->size = 1;
    cls->nbelts = 0;
    return cls;
}


t_class* createClass(int node,t_class* cls,t_adjList *adjList) {
    if (adjList == NULL) return NULL;
    int n = adjList->size;
    if (node < 1 || node > n) return NULL;

    // Initialize cls if NULL
    if (cls == NULL) {
        cls = (t_class*)malloc(sizeof(t_class));
        if (cls == NULL) return NULL;
        cls->connected = NULL;
        cls->size = 0;
        cls->nbelts = 0;
    }

    // Prepare visited array and queue for BFS
    char *visited = (char*)calloc(n, sizeof(char));
    if (visited == NULL) return cls;

    int *queue = (int*)malloc(n * sizeof(int));
    if (queue == NULL) { free(visited); return cls; }

    int head = 0, tail = 0;
    queue[tail++] = node - 1;
    visited[node - 1] = 1;

    // Ensure capacity helper
    int capacity = (cls->size > 0) ? cls->size : 4;
    if (cls->connected == NULL) {
        cls->connected = (int*)malloc(capacity * sizeof(int));
        if (cls->connected == NULL) { free(queue); free(visited); return cls; }
        cls->size = capacity;
        cls->nbelts = 0;
    }

    while (head < tail) {
        int u = queue[head++];
        // add to class list
        if (cls->nbelts >= cls->size) {
            int newcap = cls->size * 2;
            int *tmp = (int*)realloc(cls->connected, newcap * sizeof(int));
            if (tmp == NULL) break;
            cls->connected = tmp;
            cls->size = newcap;
        }
        cls->connected[cls->nbelts++] = u + 1; // store 1-based vertex

        // traverse neighbors
        t_list *lst = adjList->array[u];
        if (lst != NULL && lst->head != NULL) {
            t_cell *cur = lst->head;
            while (cur != NULL) {
                int v = cur->arrival_vertex - 1;
                if (v >= 0 && v < n && !visited[v]) {
                    visited[v] = 1;
                    queue[tail++] = v;
                }
                cur = cur->next;
            }
        }
    }

    free(queue);
    free(visited);
    return cls;
}

void displayClassConnected(t_class* cls) {
    if (cls == NULL) {
        printf("Error: class is NULL\n");
        return;
    }
    if (cls->connected == NULL || cls->nbelts == 0) {
        printf("Class is empty\n");
        return;
    }

    printf("Class connected: [");
    for (int i = 0; i < cls->nbelts; i++) {
        if (i > 0) printf(", ");
        printf("%d", cls->connected[i]);
    }
    printf("]\n");
}