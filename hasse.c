#include <malloc.h>
#include <stdio.h>

#include "hasse.h"

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->count)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->count && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.start_class == link2.start_class)
                {
                    // look for a link from link2.end_class to link1.end_class
                    int k = 0;
                    while (k < p_link_array->count && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.start_class == link2.end_class) && (link3.end_class == link1.end_class))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->count - 1];
            p_link_array->count--;
        }
        else
        {
            i++;
        }
    }
}
static int link_exists(const t_link_array *arr, int s, int e) {
    for (int i = 0; i < arr->count; ++i) {
        if (arr->links[i].start_class == s && arr->links[i].end_class == e) return 1;
    }
    return 0;
}
/*
 * Create a t_link_array from a partition and its original graph.
 * Class indices are assigned 1..m in the order encountered while traversing the partition list.
 */
t_link_array createLinkArray(t_partition *part, t_adjList *graph)
{
    t_link_array out;
    out.count = 0;
    out.capacity = 16;
    out.links = malloc(out.capacity * sizeof(t_link));
    if (!out.links) {
        out.count = out.capacity = 0;
        return out;
    }

    if (!part || !graph) return out;

    int n = graph->size;
    /* map vertex id (1..n) -> class index (1..m), 0 = none */
    int *class_of_vertex = calloc(n + 1, sizeof(int));
    if (!class_of_vertex) {
        free(out.links);
        out.links = NULL;
        out.capacity = out.count = 0;
        return out;
    }

    /* First pass: assign class indices */
    int class_idx = 1;
    t_partitionNode *pnode = part->head;
    while (pnode != NULL) {
        t_class *c = pnode->class;
        if (c) {
            t_classNode *cn = c->head;
            while (cn != NULL && cn->vertex != NULL) {
                int vid = cn->vertex->identifier;
                if (vid >= 1 && vid <= n) class_of_vertex[vid] = class_idx;
                cn = cn->next;
            }
            class_idx++;
        }
        pnode = pnode->next;
    }

    /* Second pass: for each vertex u, check its outgoing edges and add class links */
    for (int u = 1; u <= n; ++u) {
        int cu = class_of_vertex[u];
        if (cu == 0) continue;
        t_list *succs = graph->array[u - 1];
        if (!succs) continue;
        t_cell *edge = succs->head;
        while (edge) {
            int v = edge->arrival_vertex;
            if (v >= 1 && v <= n) {
                int cv = class_of_vertex[v];
                if (cv != 0 && cv != cu) {
                    /* add link cu -> cv if not already present */
                    if (!link_exists(&out, cu, cv)) {
                        if (out.count >= out.capacity) {
                            int nc = out.capacity * 2;
                            t_link *tmp = realloc(out.links, nc * sizeof(t_link));
                            if (!tmp) { /* out of memory: cleanup and return what we have */
                                free(class_of_vertex);
                                return out;
                            }
                            out.links = tmp;
                            out.capacity = nc;
                        }
                        out.links[out.count].start_class = cu;
                        out.links[out.count].end_class = cv;
                        out.count++;
                    }
                }
            }
            edge = edge->next;
        }
    }

    free(class_of_vertex);
    return out;
}

void printLinkArray(const t_link_array *arr)
{
    if (!arr) return;
    printf("Links (count = %d):\n", arr->count);
    for (int i = 0; i < arr->count; ++i) {
        printf("  C%d -> C%d\n", arr->links[i].start_class, arr->links[i].end_class);
    }
}

void freeLinkArray(t_link_array *arr)
{
    if (!arr) return;
    free(arr->links);
    arr->links = NULL;
    arr->count = arr->capacity = 0;
}

void writeHasseMermaid(t_partition *part, t_adjList *graph, const char *out_path)
{
    if (!part || !graph || !out_path) return;

    int m = 0;
    t_partitionNode *pnode = part->head;
    while (pnode) { m++; pnode = pnode->next; }
    if (m == 0) return;

    char **class_names = malloc((m + 1) * sizeof(char *));
    t_class **class_ptrs = malloc((m + 1) * sizeof(t_class *));
    if (!class_names || !class_ptrs) {
        free(class_names);
        free(class_ptrs);
        return;
    }

    int idx = 1;
    pnode = part->head;
    while (pnode) {
        t_class *c = pnode->class;
        class_ptrs[idx] = c;
        class_names[idx++] = c ? c->name : "C";
        pnode = pnode->next;
    }

    t_link_array links = createLinkArray(part, graph);
    removeTransitiveLinks(&links);

    FILE *f = fopen(out_path, "w");
    if (!f) {
        free(class_names);
        free(class_ptrs);
        freeLinkArray(&links);
        return;
    }

    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "    layout: elk\n");
    fprintf(f, "    theme: neo\n");
    fprintf(f, "    look: neo\n");
    fprintf(f, "---\n\n");

    fprintf(f, "flowchart LR\n\n");

    /* Print class nodes with only the content between braces as the label */
    for (int i = 1; i <= m; ++i) {
        char content[512];
        content[0] = '\0';

        if (class_ptrs[i] && class_ptrs[i]->head) {
            t_classNode *cn = class_ptrs[i]->head;
            int first = 1;
            while (cn != NULL) {
                if (cn->vertex != NULL) {
                    char tmp[32];
                    snprintf(tmp, sizeof(tmp), "%s%d", first ? "" : ",", cn->vertex->identifier);
                    strncat(content, tmp, sizeof(content) - strlen(content) - 1);
                    first = 0;
                }
                cn = cn->next;
            }
        }

        /* If empty content, produce an empty label inside brackets */
        fprintf(f, "    %s[%s]\n", class_names[i], content);
    }
    fprintf(f, "\n");

    for (int i = 0; i < links.count; ++i) {
        int s = links.links[i].start_class;
        int e = links.links[i].end_class;
        if (s >= 1 && s <= m && e >= 1 && e <= m) {
            fprintf(f, "    %s --> %s\n", class_names[s], class_names[e]);
        }
    }

    fclose(f);
    free(class_names);
    free(class_ptrs);
    freeLinkArray(&links);
}

/* Return 1 if class `class_idx` has at least one outgoing link in `links`. */
int isTransitory(const t_link_array *links, int class_idx)
{
    if (!links || class_idx <= 0) return 0;
    for (int i = 0; i < links->count; ++i) {
        if (links->links[i].start_class == class_idx) return 1;
    }
    return 0;
}

/* Return 1 if class `class_idx` is persistent (no outgoing links). */
int isPersistent(const t_link_array *links, int class_idx)
{
    return !isTransitory(links, class_idx);
}

/* Internal helper: find class index (1..m) containing vertex `vertex_id`, 0 if none. */
static int get_class_index_of_vertex(t_partition *part, int vertex_id)
{
    if (!part || vertex_id <= 0) return 0;
    int idx = 1;
    t_partitionNode *pnode = part->head;
    while (pnode) {
        t_class *c = pnode->class;
        if (c) {
            t_classNode *cn = c->head;
            while (cn) {
                if (cn->vertex && cn->vertex->identifier == vertex_id) return idx;
                cn = cn->next;
            }
        }
        idx++;
        pnode = pnode->next;
    }
    return 0;
}

/* Return 1 if `state_id` is absorbing: its class is persistent and contains exactly one state. */
int isAbsorbing(t_partition *part, const t_link_array *links, int state_id)
{
    if (!part || state_id <= 0) return 0;
    int cidx = get_class_index_of_vertex(part, state_id);
    if (cidx == 0) return 0;

    if (isTransitory(links, cidx)) return 0; /* not persistent */

    /* locate the class pointer and check size == 1 */
    int idx = 1;
    t_partitionNode *pnode = part->head;
    while (pnode) {
        if (idx == cidx) {
            t_class *c = pnode->class;
            if (c && c->size == 1) return 1;
            return 0;
        }
        idx++;
        pnode = pnode->next;
    }
    return 0;
}
