#include <stdlib.h>
#include <stdio.h>
#include "data.h"
char* read_data(char *filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char* nbNodes = malloc(2 * sizeof(char));
    if (!nbNodes) { fclose(file); return NULL; }

    int c = fgetc(file);
    if (c == EOF) {
        nbNodes[0] = '\0';
        nbNodes[1] = '\0';
    } else {
        nbNodes[0] = (char)c;
        nbNodes[1] = '\0';
    }
    printf("%s", nbNodes);
    char line[256];
    while (fgets(line, sizeof line, file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
    return nbNodes;
}