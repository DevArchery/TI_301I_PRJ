#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static char *getID(int i)
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

// Function that checks if a given graph is a Markov graph
void isMarkovGraph(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    int nbvert, start, end, i;
    float proba;
    int is_valid = 1; // Assumes that the graph is a Markov graph
    float sum[100] = {0.0};

    // Reads the first line which indicates the number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
        printf("Error reading file");
        fclose(file);
        return;
    }

    // Reads each line of the file and verifies the structure (start/end/probability)
    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3)
    {
        // Checks that the start is valid and add the proba to the sum
        if (start >= 1 && start <= nbvert) sum[start - 1] += proba;
    }

    // Verifies that all the probabilities of each vertex is equal to 1
    for (i = 0; i < nbvert; i++)
    {
        // If the sum is approximately equal to 1
        if (sum[i] < 0.99 || sum[i] > 1.0)
        {
            printf("The graph is not a Markov graph.\n");
            printf("The sum of the probabilities of vertex %d is %.2f\n", i+1, sum[i]);
            is_valid = 0;
        }
    }

    if (is_valid) printf("The graph is a Markov graph.\n");
    fclose(file);
}
