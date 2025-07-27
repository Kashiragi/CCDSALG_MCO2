/**
 * @file graphInfo.c
 * @description: Contains functions to load a graph from input file and export different text outputs.
 * @author Queenie Salao, VL Kirsten Camille Saguin, Kurt Anjo Laguerta
 * @tester:  Kurt Laguerta, Queenie Salao, Kei Saguin
 */

#include "globals.h"

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

void toTxt1VertexAndEdges(char output_filename[], pGraph GDS, char* graphName){
    // Check if input parameters are valid (not NULL)
    if (output_filename == NULL || GDS == NULL) return;
    
    // Open the output file in write mode
    FILE *file = fopen(output_filename, "w");
    // Check if file opening was successful
    if (file == NULL) {
        printf("Error creating output file: %s\n", output_filename);
        return;
    }
    
    // Get the total number of vertices in the graph
    int vertexCount = countVertices(GDS);
    // If no vertices exist, write empty sets and exit
	if (vertexCount == 0) {
        fprintf(file, "V(%s)={}\nE(%s)={}\n", graphName, graphName);
        fclose(file);
        return;
    }

    
    // Allocate memory for an array of string pointers to store vertex names
    char **vertices = malloc(vertexCount * sizeof(char*));
    // Check if memory allocation was successful
    if (vertices == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return;
    }
    
    // Initialize counter for array indexing
    int i = 0;
    // Get pointer to the first head vertex in the graph
    pHead current = GDS->heads;  
    // Loop through all head vertices in the graph
    while (current != NULL) {
        // Allocate memory for each individual vertex name string
        vertices[i] = malloc((MAX_ID_LEN + 1) * sizeof(char));
        // Check if memory allocation for this vertex name was successful
        if (vertices[i] == NULL) {
            printf("Memory allocation error\n");
            // Free previously allocated memory to prevent memory leaks
            for (int j = 0; j < i; j++) {
                free(vertices[j]);
            }
            free(vertices);
            fclose(file);
            return;
        }
        // Copy the vertex name from the graph structure to our array
        strcpy(vertices[i], current->name);
        // Move to the next index in our array
        i++;
        // Move to the next head vertex in the graph
        current = current->nextHead;  
    }
    
    // Sort the vertex names alphabetically using quicksort
    qsort(vertices, vertexCount, sizeof(char*), compareStrings);
    
    // Write the beginning of the vertex set notation
    fprintf(file, "V(%s)={", graphName);
    // Loop through all sorted vertices
    for (int i = 0; i < vertexCount; i++) {
        // Write the vertex name
        fprintf(file, "%s", vertices[i]);
        // Add comma separator if not the last vertex
        if (i < vertexCount - 1) {
            fprintf(file, ",");
        }
    }
    // Close the vertex set notation and add newline
    fprintf(file, "}\n");
    
    // Allocate memory for edges array (worst case: complete graph has n*n edges)
    pEdge edges = malloc(vertexCount * vertexCount * sizeof(Edge));
    // Check if memory allocation for edges was successful
    if (edges == NULL) {
        printf("Memory allocation error for edges\n");
        // Clean up previously allocated vertex memory
        for (int i = 0; i < vertexCount; i++) {
            free(vertices[i]);
        }
        free(vertices);
        fclose(file);
        return;
    }
    
    // Initialize edge counter
    int edgeCount = 0;
    // Reset current pointer to the first head vertex
    current = GDS->heads;  
    // Loop through all head vertices to collect edges
    while (current != NULL) {
        // Get pointer to the adjacency list of current vertex
        pVertex adj = current->list;  
        // Loop through all adjacent vertices
        while (adj != NULL) {
            // Only add edge once (avoid duplicates in undirected graph)
            // Use lexicographic comparison to ensure consistent ordering
            if (strcmp(current->name, adj->name) < 0) {
                // Copy the first vertex name (lexicographically smaller)
                strcpy(edges[edgeCount].vertex1, current->name);
                // Copy the second vertex name (lexicographically larger)
                strcpy(edges[edgeCount].vertex2, adj->name);
                // Increment edge counter
                edgeCount++;
            }
            // Move to next adjacent vertex
            adj = adj->next;  
        }
        // Move to next head vertex
        current = current->nextHead;
    }
    
    // Sort edges alphabetically (first by vertex1, then by vertex2)
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);
    
    // Write the beginning of the edge set notation
    fprintf(file, "E(%s)={", graphName);
    // Loop through all sorted edges
    for (int i = 0; i < edgeCount; i++) {
        // Write edge in format (vertex1,vertex2)
        fprintf(file, "(%s,%s)", edges[i].vertex1, edges[i].vertex2);
        // Add comma separator if not the last edge
        if (i < edgeCount - 1) {
            fprintf(file, ",");
        }
    }
    // Close the edge set notation
    fprintf(file, "}");
    
    // Clean up: free all allocated memory for vertex names
    for (int i = 0; i < vertexCount; i++) {
        free(vertices[i]);
    }
    // Free the array of vertex pointers
    free(vertices);
    // Free the edges array
    free(edges);
    // Close the output file
    fclose(file);
}


void toTxt2VertexDegrees(char out_filename[], pGraph GDS){
    // Initialize counter for array indexing sa loop
    int i = 0;
    // Get pointer to the first head vertex in the graph
    pHead current = GDS->heads; 
        // Get the total number of vertices in the graph
    int vertexCount = countVertices(GDS);
    // Define a local structure to hold vertex name and its degree
    typedef struct {
        char name[MAX_ID_LEN+1];    // Vertex name with null terminator space
        int degree;                 // Number of connections (adjacent vertices)
    } VertexDegree;
    // Check if input parameters are valid (not NULL)
    if (out_filename == NULL || GDS == NULL) return;
    // Open the output file in write mode
    FILE *file = fopen(out_filename, "w");

    // Check if file opening was successful
    if (file == NULL) { // if null
        printf("Error creating output file: %s\n", out_filename);
        return;
    }    

    // If no vertices exist, close file and exit
    if (vertexCount == 0) {
        fclose(file);
        return;
    }
    
    // Allocate memory for array of VertexDegree structures
    VertexDegree *vertexDegrees = malloc(vertexCount * sizeof(VertexDegree));
    // Check if memory allocation was successful
    if (vertexDegrees == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return;
    }
    
    // Loop through all head vertices in the graph
    while (current != NULL) {
        // Copy the vertex name into our structure array
        strcpy(vertexDegrees[i].name, current->name);
        
        // Initialize degree counter for this vertex
        int degree = 0;
        // Get pointer to the adjacency list of current vertex
        pVertex adj = current->list;  
        // Count all adjacent vertices (this gives us the degree)
        while (adj != NULL) {
            degree++;               // Increment degree counter
            adj = adj->next;        // Move to next adjacent vertex
        }
        // Store the calculated degree in our structure array
        vertexDegrees[i].degree = degree;
        // Move to next index in our array
        i++;
        // Move to next head vertex in the graph
        current = current->nextHead;  
    }
    
    // Sort by vertex name using bubble sort (simple but works)
    // nested loop >>
    // bubblesort
    for (int i = 0; i < vertexCount - 1; i++) {
        
        for (int j = i + 1; j < vertexCount; j++) {
            // Compare vertex names lexicographically
            if (strcmp(vertexDegrees[i].name, vertexDegrees[j].name) > 0) {
                // Swap the entire VertexDegree structures if out of order
                VertexDegree temp = vertexDegrees[i];
                vertexDegrees[i] = vertexDegrees[j];
                vertexDegrees[j] = temp;
            }
        }
    }
    
    // Write all vertex names and their degrees to the file
    for (int i = 0; i < vertexCount; i++) {
        // Write vertex name followed by space and its degree
        fprintf(file, "%s %d", vertexDegrees[i].name, vertexDegrees[i].degree);
        // Add newline if not the last vertex (avoids trailing newline)
        if (i < vertexCount - 1) {
            fprintf(file, "\n");
        }
    }
    
    // Free the allocated memory for vertex degrees array
    free(vertexDegrees);
    // Close 
    fclose(file);
}

void toTxt3AdjList(char out_filename[], pGraph g){
    // Declarations
    FILE *out;
    pHead head;
    pVertex adjacent;
    // Does nothing if graph is NULL
    if (g != NULL)
    {
        // Does nothing if file cannot be created
        out = fopen(out_filename, "w");
        if (out != NULL)
        {
            // Get first head
            head = g->heads;
            // If first head points to NULL, then the graph has no contents (\)
            if (NULL == head)
                fprintf(out, "\\");
            // Otherwise, iterate up to the last head
            else while (head != NULL)
            {
                // Write the name stored in the head vertex
                fprintf(out, "%s->", head->name);
                // Iterate through its adjacents
                adjacent = head->list;
                while (adjacent != NULL)
                {
                    fprintf(out, "%s->", adjacent->name);
                    adjacent = adjacent->next;
                }
                // Terminate adjacent list with a '\'
                if (NULL == adjacent)
                    fprintf(out, "\\"); else;
                // Write a new line and proceed to the next head vertex
                head = head->nextHead;
                if (head != NULL)
                    fprintf(out, "\n"); else;
            }
            // Finish the file
            fclose(out);
        }
    }
}

void toTxt4AdjMatrix(char out_filename[], pGraph g){
    // Variables
    FILE* outFile;
    pHead curHead = g->heads;
    pHead targetHead;
    pVertex curVertex;
    int size = g->nV;
    int i = 0, j = 0, a;
    //int adjMatrix[size][size];
    int **adjMatrix;
    adjMatrix = malloc(size * sizeof(int *));
    for (a = 0; a < size; a++)
        adjMatrix[a] = calloc(size, sizeof(int));
    // initialize with zeroes
    //memset(adjMatrix, 0, sizeof(int)*size*size);

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
        fprintf(outFile,"\t%s", curHead->name);
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
            fprintf(outFile, "\t%d", adjMatrix[i][j]);
        }
        // move on to the next head
        curHead = curHead->nextHead;
        fprintf(outFile,"\n");
    }
    fclose(outFile);
    for (a = 0; a < size; a++)
        free(adjMatrix[a]);
    free(adjMatrix);
}


int compareStrings(const void *a, const void *b) {
    char **strA = (char**)a;
    char **strB = (char**)b;
    return strcmp(*strA, *strB);
}

int compareEdges(const void *a, const void *b) {
    pEdge edgeA = (pEdge)a;
    pEdge edgeB = (pEdge)b;
    
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

void getGraphNameFromFilename(char *filename, char *graphName) {
    char *base = strrchr(filename, '/'); // handles path like ./data/A.TXT
    base = (base) ? base + 1 : filename;

    strncpy(graphName, base, MAX_ID_LEN);
    graphName[MAX_ID_LEN] = '\0';

    char *dot = strchr(graphName, '.');
    if (dot) *dot = '\0'; // strip file extension
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
