/**
 * @file graphInfo.c
 * @description Contains functions to load a graph from input file and export different text outputs.
 * @author Queenie Salao, Kei Saguin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphInfo.h"

/**
 * Reads and parses a graph from a formatted text file.
 * Populates the graph structure with vertex and edge info.
 *
 * @author Queenie Salao
 * @param input_filename Name of the input .TXT file.
 * @param GDS Pointer to graph structure to fill.
 * @return 1 if successful, 0 otherwise.
 */
int readInputFile(char input_filename[], pGraph GDS){
    // Early return if param values are NULL
    if (input_filename == NULL || GDS == NULL) return 0;
    
    // Variable declarations
    int temp;
    int numVertices;
    char line[1000];
    char* token;
    char headVertex[MAX_ID_LEN+1];
    char adjVertex[MAX_ID_LEN+1];
    pHead curHead;

    FILE *file = fopen(input_filename, "r");

    // early returns if filename doesn't exist, or problems reading the .txt file
    if (file == NULL) {
        printf("File %s not found.\n", input_filename);
        return 0;
    }
    if (fscanf(file, "%d", &numVertices) != 1) {
        printf("Error reading number of vertices.\n");
        fclose(file);
        return 0;
    }
    // assign scanned number of vertices
    GDS->nV = numVertices;
    
    // get next line after number of vertices line
    fgets(line, sizeof(line), file); 
    
    // continue loop while the scanner can still get a line
    while (fgets(line, sizeof(line), file)) {
        // take the first token of the line based on delimiters " \t\n"
        token = strtok(line, " \t\n");
        if (token == NULL) continue;
        
        // copy scanned string into headVertex[]
        strncpy(headVertex, token, MAX_ID_LEN);
        headVertex[MAX_ID_LEN] = '\0'; 
        
        // check if head already exists
        curHead = findHead(GDS, headVertex, &temp);
        // if it doesn't exist (hence NULL)
        if (curHead == NULL) {
            // add to graph
            curHead = addHead(GDS, headVertex);
            // if after adding, it returns null
            if (curHead == NULL) {
                printf("Error adding head: %s\n", headVertex);
                fclose(file);
                return 0;
            }
        }
        // take the next token of the line based on delimiters
        token = strtok(NULL, " \t\n");
        // continue loop until the line still has tokens and the 
        // token is not "-1"
        while (token != NULL && strcmp(token, "-1") != 0) {
            // copy scanned string into adjVertex[]
            strncpy(adjVertex, token, MAX_ID_LEN);
            adjVertex[MAX_ID_LEN] = '\0'; 
            // add adjacent to the head
            addAdjacent(curHead, adjVertex);
            // take the next token of the line based on delimiters
            token = strtok(NULL, " \t\n");
        }
    }
    fclose(file);
    return 1;
}

// first output file
void toTxt1VertexAndEdges(char output_filename[], pGraph GDS){
    /*
        G-SET.TXT contains:
        V(G)={Bruce,Clark,Diana,Hal}
        E(G)={(Bruce,Diana),(Clark,Diana),(Clark,Hal),(Diana,Hal)}
    */
    if (output_filename == NULL || GDS == NULL) return;
    
    FILE *file = fopen(output_filename, "w");
    if (file == NULL) {
        printf("Error creating output file: %s\n", output_filename);
        return;
    }
    
    int vertexCount = countVertices(GDS);
    if (vertexCount == 0) {
        fprintf(file, "V(G)={}\nE(G)={}");
        fclose(file);
        return;
    }
    
    char **vertices = malloc(vertexCount * sizeof(char*));
    // char (*vertices)[MAX_ID_LEN+1] = calloc(vertexCount, sizeof(char[MAX_ID_LEN+1]));
    if (vertices == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return;
    }
    
    int i = 0;
    pHead current = GDS->heads;  
    while (current != NULL) {
        vertices[i] = malloc((MAX_ID_LEN + 1) * sizeof(char));
        if (vertices[i] == NULL) {
            printf("Memory allocation error\n");
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(vertices[j]);
            }
            free(vertices);
            fclose(file);
            return;
        }
        strcpy(vertices[i], current->name);
        i++;
        current = current->nextHead;  
    }
    
    qsort(vertices, vertexCount, sizeof(char*), compareStrings);
    
    fprintf(file, "V(G)={");
    for (int i = 0; i < vertexCount; i++) {
        fprintf(file, "%s", vertices[i]);
        if (i < vertexCount - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "}\n");
    
    pEdge edges = malloc(vertexCount * vertexCount * sizeof(Edge));
    if (edges == NULL) {
        printf("Memory allocation error for edges\n");
        for (int i = 0; i < vertexCount; i++) {
            free(vertices[i]);
        }
        free(vertices);
        fclose(file);
        return;
    }
    
    int edgeCount = 0;
    current = GDS->heads;  
    while (current != NULL) {
        pVertex adj = current->list;  
        while (adj != NULL) {
            // Only add edge once (avoid duplicates in undirected graph)
            if (strcmp(current->name, adj->name) < 0) {
                strcpy(edges[edgeCount].vertex1, current->name);
                strcpy(edges[edgeCount].vertex2, adj->name);
                edgeCount++;
            }
            adj = adj->next;  
        }
        current = current->nextHead;
    }
    
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);
    
    fprintf(file, "E(G)={");
    for (int i = 0; i < edgeCount; i++) {
        fprintf(file, "(%s,%s)", edges[i].vertex1, edges[i].vertex2);
        if (i < edgeCount - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "}");
    
    for (int i = 0; i < vertexCount; i++) {
        free(vertices[i]);
    }
    free(vertices);
    free(edges);
    fclose(file);
}

void toTxt2VertexDegrees(char out_filename[], pGraph GDS){
    /*
        G-DEGREE.TXT contains
        Bruce 1
        Clark 2
        Diana 3
        Hal 2

    */
    if (out_filename == NULL || GDS == NULL) return;
    
    FILE *file = fopen(out_filename, "w");
    if (file == NULL) {
        printf("Error creating output file: %s\n", out_filename);
        return;
    }
    
    typedef struct {
        char name[MAX_ID_LEN+1];
        int degree;
    } VertexDegree;
    
    int vertexCount = countVertices(GDS);
    if (vertexCount == 0) {
        fclose(file);
        return;
    }
    
    VertexDegree *vertexDegrees = malloc(vertexCount * sizeof(VertexDegree));
    if (vertexDegrees == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return;
    }
    
    int i = 0;
    pHead current = GDS->heads;  
    while (current != NULL) {
        strcpy(vertexDegrees[i].name, current->name);
        
        int degree = 0;
        pVertex adj = current->list;  
        while (adj != NULL) {
            degree++;
            adj = adj->next;  
        }
        vertexDegrees[i].degree = degree;
        i++;
        current = current->nextHead;  
    }
    
    // Sort by vertex name using bubble sort (simple but works)
    for (int i = 0; i < vertexCount - 1; i++) {
        for (int j = i + 1; j < vertexCount; j++) {
            if (strcmp(vertexDegrees[i].name, vertexDegrees[j].name) > 0) {
                VertexDegree temp = vertexDegrees[i];
                vertexDegrees[i] = vertexDegrees[j];
                vertexDegrees[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < vertexCount; i++) {
        fprintf(file, "%s %d", vertexDegrees[i].name, vertexDegrees[i].degree);
        if (i < vertexCount - 1) {
            fprintf(file, "\n");
        }
    }
    
    free(vertexDegrees);
    fclose(file);
}

void toTxt3AdjList(char out_filename[], pGraph g){
    /*
        G-LIST.TXT contains
        Diana->Hal->Bruce->Clark->\
        Bruce->Diana->\
        Hal->Clark->Diana->\
        Clark->Hal->Diana->\
    */
    FILE* outFile;
    pHead curhead = g->heads;
    outFile = fopen(out_filename, "w");

    while(curhead!=NULL){
        pVertex curvertex = curhead->list;
        fprintf(outFile, "%s->", curhead->name);
        while(curvertex!=NULL){
            fprintf(outFile,"%s->", curvertex->name);
            curvertex = curvertex->next;
        }
        fprintf(outFile, "\\\n");
        
        curhead = curhead->nextHead;
    }
    fclose(outFile);

}

/**
 *  Takes the graph and converts it into an adjacency matrix (within this function only).
 *  Prints the matrix out to the text file specified
 * 
 *  @author VL Kirsten Camille D. Saguin
 *  @param out_filename the filename of the output file of the adjacency matrix
 *  @param g the graph structure containing information about the inputted graph
 */
void toTxt4AdjMatrix(char out_filename[], pGraph g){
    /*
        G-MATRIX.TXT contains
                Diana Bruce Hal Clark
        Diana   0     1     1   1
        Bruce   1     0     0   0
        Hal     1     0     0   1
        Clark   1     0     1   0
    */
    // Variables
    FILE* outFile;
    pHead curHead = g->heads;
    pHead targetHead;
    pVertex curVertex;
    int size = g->nV;
    int i = 0, j = 0;
    int adjMatrix[size][size];
    // initialize with zeroes
    memset(adjMatrix, 0, sizeof(adjMatrix[0][0])*size*size);

    // Check all heads
    while (curHead!=NULL){
        curVertex = curHead->list;

        // Check all neighbors or adjacent vertices of this head
        while(curVertex!=NULL){
            // Check if adjacent vertex exists as a head (it should be due to adj list)
            // and get its index (j)
            targetHead = findHead(g, curVertex->name,&j);
            // If the vertex exists as a head and j returns the index
            if(targetHead!=NULL || j!=-1){
                // since undirected, both ways is marked as 1
                adjMatrix[i][j] = 1;
                adjMatrix[j][i] = 1;
            }
            // move on to the next neighbor of this head
            curVertex = curVertex->next;
            
        }
        // move on to the next head
        curHead = curHead->nextHead;
        i++;
    }
    
    // === outputing to .txt file ===
    outFile = fopen(out_filename,"w");
    // resetting curhead to point to the first head of the graph
    curHead = g->heads;
    while(curHead!=NULL){
        // print the heads as column headers, in sequence
        fprintf(outFile, "\t\t%s", curHead->name);
        curHead = curHead->nextHead;
    }

    fprintf(outFile, "\n");
    // resetting curhead to point to the first head of the graph
    curHead = g->heads;
    for(int i=0; i<size;i++){
        // print the head name as row header
        fprintf(outFile, "%s", curHead->name);
        for(int j=0; j<size; j++){
            // print all matrix values for that head (ith row)
            fprintf(outFile, "\t\t%d", adjMatrix[i][j]);
        }
        // move on to the next head
        curHead = curHead->nextHead;
        fprintf(outFile,"\n");
    }
    fclose(outFile);
}

/**
 *  Compares the value of two given strings. Helper function 
 *  used for qsort in toTxt1VertexAndEdges.
 *  
 *  @author Queenie Salao
 *  @param a address of the first string pointer
 *  @param b address of the second string pointer
 *  @return negative if *a < *b, positive if *a > *b, zero if equal
 */
int compareStrings(const void *a, const void *b) {
    char **strA = (char**)a;
    char **strB = (char**)b;
    return strcmp(*strA, *strB);
}
/**
 *  Compares the value of two given strings. Helper function 
 *  used for qsort in toTxt1VertexAndEdges.
 *  
 *  @author Queenie Salao
 *  @param a address of the first edge pointer
 *  @param b address of the second edge pointer
 *  @return negative if *a < *b, positive if *a > *b, zero if equal
 */
int compareEdges(const void *a, const void *b) {
    pEdge edgeA = (pEdge)a;
    pEdge edgeB = (pEdge)b;
    
    int cmp = strcmp(edgeA->vertex1, edgeB->vertex1);
    if (cmp == 0) {
        cmp = strcmp(edgeA->vertex2, edgeB->vertex2);
    }
    return cmp;
}

/**
 *  Returns the number of vertices of the graph
 *  @author Queenie Salao
 *  @return the number of vertices of the graph 
 */
int countVertices(pGraph graph) {
    if (graph == NULL) return 0;
    return graph->nV;
}
/**
 * Creates output file name by appending suffix to input filename.
 *
 * @author Queenie Salao
 * @param inputFileName Original filename (e.g. G.TXT)
 * @param suffix Suffix to add (e.g. -SET)
 * @param outputFileName Output filename with suffix and .TXT
 */
void createOutputFileName(char* inputFileName, char* suffix, char* outputFileName) {
    if (inputFileName == NULL || suffix == NULL || outputFileName == NULL) return;
    
    char* dotPos = strrchr(inputFileName, '.');
    if (dotPos != NULL) {
        int baseLength = dotPos - inputFileName;
        strncpy(outputFileName, inputFileName, baseLength);
        outputFileName[baseLength] = '\0';
        
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    } else {
        strcpy(outputFileName, inputFileName);
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    }
}
// int main() {
//     char inputFileName[100]; 
    
//     printf("Input filename: ");
//     if (scanf("%99s", inputFileName) != 1) { 
//         printf("Error reading filename.\n");
//         return 1;
//     }
    
//     pGraph graph = gcreate(1); //!initial initialization
//     // if (graph == NULL) {
//     //     printf("Error creating graph.\n");
//     //     return 1;
//     // }
    
//     if (!readInputFile(inputFileName, graph)) {
//         printf("Error reading input file.\n");
//         freeGraph(graph);
//         return 1;
//     }
//     //debug
//     printGraph(graph);
//     printf("%d", graph->nV);
//     //output
//     char output1[150], output2[150]; 
//     createOutputFileName(inputFileName, "-SET", output1);
//     createOutputFileName(inputFileName, "-DEGREE", output2);
    
//     // output 1 n 2
//     toTxt1VertexAndEdges(output1, graph);
//     toTxt2VertexDegrees(output2, graph);
    
//     printf("Generated: %s, %s\n", output1, output2);
    
//     freeGraph(graph);
    
//     return 0;
// } 
