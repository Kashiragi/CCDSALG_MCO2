// contains code related to graph
// #include <string.h>
// #include "graph.c"
#include "globals.h"

typedef struct {
    char vertex1[MAX_ID_LEN+1];
    char vertex2[MAX_ID_LEN+1];
}*Edge;

int compareStrings(const void *a, const void *b);

int compareEdges(const void *a, const void *b);

int countVertices(pGraph graph);

void createOutputFileName(char* inputFileName, char* suffix, char* outputFileName);
// function to read, and process .txt file into graph representation (list or matrix)
// change void later to idk Graph return type so its reusable in main
int readInputFile(char input_filename[], pGraph GDS);

// first output file
void toTxt1VertexAndEdges(char output_filename[], pGraph GDS);

void toTxt2VertexDegrees(char out_filename[], pGraph GDS);

void toTxt3AdjList(char out_filename[], pGraph GDS);
//@author Kei
void toTxt4AdjMatrix(char out_filename[], pGraph g);