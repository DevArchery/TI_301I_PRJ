#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "utils.h"
#include "hasse.h"
#include "matrix.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

// forward declaration (with optional extension filter, pass NULL to accept all)
static int list_dir_files(const char* dir, const char* ext, char out[][256], int max);

// Simple helper to print a banner / placeholder for ASCII art
static void printBanner(void) {
    // Project ASCII art banner
    printf("\n");
    printf("    .___  ___.      ___      .______       __  ___   ______   ____    ____      ______  __    __       ___       __  .__   __.      _______.\n");
    printf("    |   \\/   |     /   \\     |   _  \\     |  |/  /  /  __  \\  \\   \\  /   /     /      ||  |  |  |     /   \\     |  | |  \\ |  |     /       |\n");
    printf("    |  \\  /  |    /  ^  \\    |  |_)  |    |  '  /  |  |  |  |  \\   \\ \\/   /     |  ,----'|  |__|  |    /  ^  \\    |  | |   \\|  |    |   (----`\n");
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
                ; // empty statement to allow declarations
                if (graph) printf("Replacing previously loaded graph.\n");

                // List files from data/ and test_bench/ and let the user choose
                {
                    const int MAX_FILES = 512;
                    char (*files)[256] = malloc(MAX_FILES * sizeof *files);
                    if (!files) { printf("Memory allocation failed while listing files.\n"); break; }
                    int counts[2] = {0,0};
                    int total = 0;
                    counts[0] = list_dir_files("data", ".txt", files + total, MAX_FILES - total);
                    total += counts[0];
                    counts[1] = list_dir_files("test_bench", ".txt", files + total, MAX_FILES - total);
                    total += counts[1];

                    if (total == 0) {
                        // fallback to manual path input
                        printf("No files found in 'data/' or 'test_bench/'. Enter path (empty for default '%s'): ", default_path);
                        int ch2;
                        while ((ch2 = getchar()) != '\n' && ch2 != EOF) {}
                        if (fgets(path, sizeof(path), stdin) == NULL) {
                            strncpy(path, default_path, sizeof(path));
                            path[sizeof(path)-1] = '\0';
                        }
                        size_t len2 = strlen(path);
                        if (len2 > 0 && path[len2-1] == '\n') path[len2-1] = '\0';
                        graph = readGraph(path);
                        if (!graph) printf("Failed to load graph from '%s'\n", path);
                        else printf("Graph loaded from '%s'\n", path);
                        break;
                    }

                    printf("Available files:\n");
                    for (int i = 0; i < total; ++i) {
                        printf(" %3d) %s\n", i+1, files[i]);
                    }
                    printf("Choose a file number (0 to cancel): ");
                    int sel = -1;
                    if (scanf("%d", &sel) != 1) {
                        int c; while ((c = getchar()) != '\n' && c != EOF) {}
                        printf("Invalid selection.\n");
                        break;
                    }
                    if (sel <= 0 || sel > total) { printf("Canceled or invalid selection.\n"); free(files); break; }

                    // Construct chosen path
                    strncpy(path, files[sel-1], sizeof(path));
                    path[sizeof(path)-1] = '\0';
                    graph = readGraph(path);
                    if (!graph) printf("Failed to load graph from '%s'\n", path);
                    else printf("Graph loaded from '%s'\n", path);
                    free(files);
                }
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
                    int ch; // input scratch
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

// Populate up to max files found in a directory. Each entry is written as "dir/filename" into out[i].
static int list_dir_files(const char* dir, const char* ext, char out[][256], int max) {
    int count = 0;
#ifdef _WIN32
    char pattern[512];
    snprintf(pattern, sizeof(pattern), "%s\\*", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        // If ext filter provided, check file extension (case-insensitive)
        if (ext) {
            const char* name = fd.cFileName;
            size_t ln = strlen(name);
            size_t le = strlen(ext);
            if (ln < le) continue;
            // compare end of name to ext (case-insensitive)
            const char* tail = name + ln - le;
            int match = _stricmp(tail, ext) == 0;
            if (!match) continue;
        }
        snprintf(out[count], 256, "%s/%s", dir, fd.cFileName);
        count++;
        if (count >= max) break;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
#else
    DIR *d = opendir(dir);
    if (!d) return 0;
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL && count < max) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        // skip directories when possible (not portable to check d_type everywhere)
#ifdef DT_DIR
        if (ent->d_type == DT_DIR) continue;
#endif
        if (ext) {
            const char* name = ent->d_name;
            size_t ln = strlen(name);
            size_t le = strlen(ext);
            if (ln < le) continue;
            const char* tail = name + ln - le;
            // case-insensitive compare
            int match = 1;
            for (size_t i = 0; i < le; ++i) {
                char a = tail[i]; char b = ext[i];
                if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
                if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
                if (a != b) { match = 0; break; }
            }
            if (!match) continue;
        }
        snprintf(out[count], 256, "%s/%s", dir, ent->d_name);
        count++;
    }
    closedir(d);
#endif
    return count;
} 