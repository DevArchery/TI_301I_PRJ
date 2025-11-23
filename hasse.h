#ifndef __HASSE_H__
#define __HASSE_H__

#include "utils.h"

// Structure to represent a single link between two classes
typedef struct {
    int start_class;
    int end_class;
} t_link;

// Structure to hold an array of links
typedef struct {
    t_link *links;
    int count;
    int capacity;
} t_link_array;

void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */
static int link_exists(const t_link_array *arr, int s, int e);
t_link_array createLinkArray(t_partition *part, t_adjList *graph);
void printLinkArray(const t_link_array *arr);
void freeLinkArray(t_link_array *arr);
void writeHasseMermaid(t_partition *part, t_adjList *graph, const char *out_path);

int is_class_transitory(const t_link_array *links, int class_idx);
int is_class_persistent(const t_link_array *links, int class_idx);
int is_state_absorbing(t_partition *part, const t_link_array *links, int state_id);


#endif