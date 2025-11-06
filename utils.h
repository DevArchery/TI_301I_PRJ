#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function prototypes from utils.c
char *getID(int);


typedef struct t_cell {
    int arrival_vertex;
    int probability;
    struct t_cell *next;
} t_cell;
/***
 * @brief create a cell
 * @param arrival_vertex : vertex where the cell arrives
 * @param prob : probability of the cell
 * @return : pointer to the created cell
 */

t_cell* createCell(int, int);



typedef struct t_list {
    t_cell *head;
} t_list;
/***
 * @brief create an empty list
 * @return : pointer to the created empty list
 */

t_list* createEmptyList(void);
t_list* addCellList(t_list*, t_cell*);
void displayList(t_list*);



t_list** createEmptyAdjencyList(int);
void displayAdjList(t_list**, int);


#ifdef __cplusplus
}
#endif

#endif