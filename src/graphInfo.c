// contains code related to graph
// #include <string.h>
// #include "graph.c"
// #include "globals.h"
#include "graphInfo.h"

// function to read, and process .txt file into graph representation (list or matrix)
// change void later to idk Graph return type so its reusable in main
int readInputFile(char input_filename[], pGraph GDS){
    if (input_filename == NULL || GDS == NULL) return 0;
    
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) {
        printf("File %s not found.\n", input_filename);
        return 0;
    }
    
    int numVertices;
    if (fscanf(file, "%d", &numVertices) != 1) {
        printf("Error reading number of vertices.\n");
        fclose(file);
        return 0;
    }
    
    char line[1000];
    fgets(line, sizeof(line), file); 
    
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t\n");
        if (token == NULL) continue;
        
        char headVertex[MAX_ID_LEN+1];
        strncpy(headVertex, token, MAX_ID_LEN);
        headVertex[MAX_ID_LEN] = '\0'; 
        
        pHead head = findHead(GDS, headVertex, NULL);
        if (head == NULL) {
            head = addHead(GDS, headVertex);
            if (head == NULL) {
                printf("Error adding vertex: %s\n", headVertex);
                fclose(file);
                return 0;
            }
        }
        
        token = strtok(NULL, " \t\n");
        while (token != NULL && strcmp(token, "-1") != 0) {
            char adjVertex[MAX_ID_LEN+1];
            strncpy(adjVertex, token, MAX_ID_LEN);
            adjVertex[MAX_ID_LEN] = '\0'; 
            
            pHead adjHead = findHead(GDS, adjVertex, NULL);
            if (adjHead == NULL) {
                adjHead = addHead(GDS, adjVertex);
                if (adjHead == NULL) {
                    printf("Error adding vertex: %s\n", adjVertex);
                    fclose(file);
                    return 0;
                }
            }
            
            addAdjacent(head, adjVertex);
            addAdjacent(adjHead, headVertex);
            
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
    
    char *vertices = malloc(vertexCount * sizeof(char));
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
    
    Edge edges = malloc(vertexCount * vertexCount * sizeof(Edge));
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

void toTxt3AdjList(char out_filename[], pGraph GDS){
    /*
        G-LIST.TXT contains
        Diana->Hal->Bruce->Clark->\
        Bruce->Diana->\
        Hal->Clark->Diana->\
        Clark->Hal->Diana->\
    */

}
//@author Kei
void toTxt4AdjMatrix(char out_filename[], pGraph g){
    /*
        G-MATRIX.TXT contains
                Diana Bruce Hal Clark
        Diana   0     1     1   1
        Bruce   1     0     0   0
        Hal     1     0     0   1
        Clark   1     0     1   0
    */
    FILE* outFile;
    pHead curHead = g->heads;
    pHead targetHead;
    pVertex curVertex;
    int size = g->nV;
    int i = 0, j = 0;
    int adjMatrix[size][size];
    //initialize with zeroes
    memset(adjMatrix, 0, sizeof(adjMatrix[0][0])*size*size);
    while (curHead!=NULL){
        curVertex = curHead->list;
        while(curVertex!=NULL){
            targetHead = findHead(g, curVertex->name,&j);
            //for debugging
            // printf("+%d+\n",j);
            if(targetHead!=NULL || j!=-1){
                adjMatrix[i][j] = 1;
                adjMatrix[j][i] = 1;
            }
            curVertex = curVertex->next;
            
        }
        curHead = curHead->nextHead;
        i++;
    }
    
    //outputing to .txt file
    outFile = fopen(out_filename,"w");

    curHead = g->heads;//resettign pointer
    while(curHead!=NULL){
        // printf("\t\t%s", curHead->name);
        fprintf(outFile, "\t\t%s", curHead->name);
        curHead = curHead->nextHead;
    }

    // printf("\n");
    fprintf(outFile, "\n");

    curHead = g->heads;//resetting pointer
    for(int i=0; i<g->nV;i++){
        // printf("%s", curHead->name);
        fprintf(outFile, "%s", curHead->name);
        for(int j=0; j<g->nV; j++){
            // printf("\t\t%d", adjMatrix[i][j]);
            fprintf(outFile, "\t\t%d", adjMatrix[i][j]);
        }
        curHead = curHead->nextHead;
        // printf("\n");
        fprintf(outFile,"\n");
    }
    fclose(outFile);
}

int compareStrings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int compareEdges(const void *a, const void *b) {
    Edge edgeA = (Edge)a;
    Edge edgeB = (Edge)b;
    
    int cmp = strcmp(edgeA->vertex1, edgeB->vertex1);
    if (cmp == 0) {
        cmp = strcmp(edgeA->vertex2, edgeB->vertex2);
    }
    return cmp;
}

int countVertices(pGraph graph) {
    if (graph == NULL) return 0;
    return graph->nV;
}

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