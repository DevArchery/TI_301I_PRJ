#include <stdio.h>
#include "data.h"
#include "utils.h"

int main() {
    read_data("../data/exemple1.txt");

    int array_size = 5;
    t_cell** cellArray = createCellArray(array_size);

    if (cellArray != NULL) {
        // Create individual cells and assign them
        cellArray[0] = createCell(1, 10);
        cellArray[1] = createCell(2, 20);
        cellArray[2] = createCell(3, 30);
        // cellArray[3] and cellArray[4] remain NULL

        // Use the cells
        if (cellArray[0] != NULL) {
            printf("Cell 0: vertex=%d, prob=%d\n",
            cellArray[0]->arrival_vertex, cellArray[0]->probability);
        }

        // Clean up
        for (int i = 0; i < array_size; i++) {
            if (cellArray[i] != NULL) {
                free(cellArray[i]);
            }
        }
        free(cellArray);
        }
    return 0;
}
