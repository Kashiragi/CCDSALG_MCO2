#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "graphInfo.c"     // using .c directly to avoid changing headers
#include "traversal.c"     // includes bfs and dfs traversal logic

int main() {
    char inputFile[100], baseName[100], startVertex[MAX_ID_LEN + 1];

    printf("Input filename: ");
    scanf("%s", inputFile);

    FILE *fileCheck = fopen(inputFile, "r");
    if (fileCheck == NULL) {
        printf("File %s not found.\n", inputFile);
        return 1;
    }
    fclose(fileCheck);

    // Extract graph name (e.g., G from G.TXT)
    strncpy(baseName, inputFile, strcspn(inputFile, "."));
    baseName[strcspn(inputFile, ".")] = '\0';

    // Create graph and load from file
    pGraph G = gcreate(1);
    if (!readInputFile(inputFile, G)) {
        freeGraph(G);
        return 1;
    }

    // Output files 1�4
    char out1[120], out2[120], out3[120], out4[120];
    createOutputFileName(inputFile, "-SET", out1);
    createOutputFileName(inputFile, "-DEGREE", out2);
    createOutputFileName(inputFile, "-LIST", out3);
    createOutputFileName(inputFile, "-MATRIX", out4);

    toTxt1VertexAndEdges(out1, G);
    toTxt2VertexDegrees(out2, G);
    toTxt3AdjList(out3, G);
    toTxt4AdjMatrix(out4, G);

    // Prompt for traversal start vertex
    printf("Input start vertex for the traversal: ");
    scanf("%s", startVertex);

    int index;
    if (findHead(G, startVertex, &index) == NULL) {
        printf("Vertex %s not found.\n", startVertex);
        freeGraph(G);
        return 1;
    }

    // Output files 5 & 6
    char out5[120], out6[120];
    createOutputFileName(inputFile, "-BFS", out5);
    createOutputFileName(inputFile, "-DFS", out6);

    toTxt5_BFS_Traversal(out5, G, startVertex);
    toTxt6_DFS_Traversal(out6, G, startVertex);

    freeGraph(G);
    return 0;
}

