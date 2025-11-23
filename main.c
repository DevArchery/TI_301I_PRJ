#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "utils.h"
#include "hasse.h"
#include "matrix.h"

// Simple helper to print a banner / placeholder for ASCII art
static void printBanner(void) {
    // Project ASCII art banner
    printf("\n");
    printf("    .___  ___.      ___      .______       __  ___   ______   ____    ____      ______  __    __       ___       __  .__   __.      _______.\n");
    printf("    |   \\/   |     /   \\     |   _  \\     |  |/  /  /  __  \\  \\   \\  /   /     /      ||  |  |  |     /   \\     |  | |  \\ |  |     /       |\n");
    printf("    |  \\  /  |    /  ^  \\    |  |_)  |    |  '  /  |  |  |  |  \\   \\/   /     |  ,----'|  |__|  |    /  ^  \\    |  | |   \\|  |    |   (----`\n");
    printf("    |  |\\/|  |   /  /_\\  \\   |      /     |    <   |  |  |  |   \\      /      |  |     |   __   |   /  /_\\  \\   |  | |  . `  |     \\   \\    \n");
    printf("    |  |  |  |  /  _____  \\  |  |\\  \\----.|  .  \\  |  `--'  |    \\    /       |  `----.|  |  |  |  /  _____  \\  |  | |  |\\   | .----)   |   \n");
    printf("    |__|  |__| /__/     \\__\\ | _| `._____||__|\\__\\  \\______/      \\__/         \\______||__|  |__| /__/     \\__\\ |__| |__| \\__| |_______/    \n");
    printf("                                                                                                                                            \n\n");
}

static void printMenu(void) {
    printf("Menu:\n");
    printf(" 1) Load graph from file\n");
    printf(" 2) Display adjacency list\n");
    printf(" 3) Check Markov validity\n");
    printf(" 4) Generate Mermaid output\n");
    printf(" 5) Run Tarjan (SCC) and display partition\n");
    printf(" 6) Build Hasse links (create/clean/print/write)\n");
    printf(" 7) Convert adjacency to matrix and print\n");
    printf(" 8) Matrix operations (power, multiply, diff)\n");
    printf(" 9) Exit\n");
    printf("Choose an option: ");
}

static void printMatrix(matrix *M) {
    if (M == NULL || M->proba == NULL) {
        printf("(empty matrix)\n");
        return;
    }
    printf("Matrix %dx%d:\n", M->rows, M->columns);
    for (int i = 0; i < M->rows; ++i) {
        for (int j = 0; j < M->columns; ++j) {
            printf("%.4f ", M->proba[i][j]);
        }
        printf("\n");
    }
}

static void freeMatrixLocal(matrix *M) {
    if (M == NULL) return;
    if (M->proba) {
        for (int i = 0; i < M->rows; ++i) {
            free(M->proba[i]);
        }
        free(M->proba);
        M->proba = NULL;
    }
    M->rows = M->columns = 0;
}

int main(void) {
    char default_path[] = "../data/exemple_valid_step3.txt";
    char path[512];
    int choice = 0;
    t_adjList* graph = NULL;
    t_partition* part = NULL;

    printBanner();

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            // clear invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input, try again.\n");
            continue;
        }

        switch (choice) {
            case 1: // Load graph
                printf("Enter path to graph file (empty for default '%s'): ", default_path);
                // clear newline
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                if (fgets(path, sizeof(path), stdin) == NULL) {
                    strncpy(path, default_path, sizeof(path));
                    path[sizeof(path)-1] = '\0';
                }
                // strip newline
                size_t len = strlen(path);
                if (len == 0 || (len == 1 && path[0] == '\n')) {
                    strncpy(path, default_path, sizeof(path));
                } else {
                    if (path[len-1] == '\n') path[len-1] = '\0';
                }
                if (graph) {
                    printf("Replacing previously loaded graph.\n");
                    // no generic freeGraph() available here; user may re-run program to fully free
                }
                graph = readGraph(path);
                if (!graph) printf("Failed to load graph from '%s'\n", path);
                else printf("Graph loaded from '%s'\n", path);
                break;

            case 2: // Display adjacency list
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                displayAdjList(graph);
                break;

            case 3: // Check Markov validity
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                checkMarkovValidity(graph);
                break;

            case 4: // Mermaid output
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                mermaidOutput(graph);
                break;

            case 5: // Tarjan
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                part = TarjanAlgorithm(graph);
                if (part == NULL) {
                    printf("Tarjan failed or returned empty partition.\n");
                } else {
                    displayPartition(part);
                }
                break;

            case 6: // Hasse processing
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                if (!part) {
                    printf("No partition available - running Tarjan now.\n");
                    part = TarjanAlgorithm(graph);
                }
                if (!part) { printf("Cannot proceed without partition.\n"); break; }
                {
                    t_link_array links = createLinkArray(part, graph);
                    removeTransitiveLinks(&links);
                    printLinkArray(&links);
                    writeHasseMermaid(part, graph, "../hasse_output.txt");
                    freeLinkArray(&links);
                }
                break;

            case 7: // Convert to matrix
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                {
                    matrix M = adjencyListToMatrix(graph);
                    printMatrix(&M);
                    freeMatrixLocal(&M);
                }
                break;

            case 8: // Matrix operations
                if (!graph) { printf("No graph loaded. Use option 1.\n"); break; }
                {
                    matrix M = adjencyListToMatrix(graph);
                    if (!M.proba) { printf("Failed to build matrix.\n"); break; }
                    printMatrix(&M);
                    printf("Choose matrix op: 1) element-wise power 2) multiply by itself 3) diff (L1) vs powered\n");
                    int mop=0;
                    if (scanf("%d", &mop) != 1) { while ((ch = getchar()) != '\n' && ch != EOF) {} ; printf("Invalid op\n"); freeMatrixLocal(&M); break; }
                    if (mop == 1) {
                        float p = 1.0f;
                        printf("Enter power (float): ");
                        if (scanf("%f", &p) != 1) { printf("Invalid power\n"); }
                        else {
                            matrix P = matrixToPower(M, p);
                            printMatrix(&P);
                            freeMatrixLocal(&P);
                        }
                    } else if (mop == 2) {
                        matrix R = multiplyMatrix(M, M);
                        printMatrix(&R);
                        freeMatrixLocal(&R);
                    } else if (mop == 3) {
                        float p = 1.0f;
                        printf("Enter power to compare against (float): ");
                        if (scanf("%f", &p) != 1) { printf("Invalid power\n"); }
                        else {
                            matrix P = matrixToPower(M, p);
                            float d = matrixDiff(M, P);
                            printf("L1 diff between original and powered matrix = %.6f\n", d);
                            freeMatrixLocal(&P);
                        }
                    } else {
                        printf("Unknown matrix op.\n");
                    }
                    freeMatrixLocal(&M);
                }
                break;

            case 9:
                printf("Exiting.\n");
                return 0;

            default:
                printf("Unknown choice.\n");
                break;
        }

        printf("\n");
    }

    return 0;
}