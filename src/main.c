#include "graph.c"
#include "graphInfo.c"
#include "traversal.c"

/**
 * Main entry point of the program.
 * Loads an undirected graph from file, generates outputs including
 * vertex/edge set, vertex degrees, adjacency list and matrix,
 * and BFS/DFS traversals starting from a specified vertex.
 *
 * @author  Queenie Salao
 * @param   none
 * @return  int (0 on success, 1 on error)
 * @tester  Kurt Laguerta, Queenie Salao, Kei Saguin
 */
int main() {
    // ==== Variable Declarations ====

    // Input handling
    char inputFile[100];                       /**< Input filename (e.g., G.TXT) */
    char startVertex[MAX_ID_LEN + 1];          /**< Start vertex for BFS and DFS */
    FILE *fileCheck;                           /**< File pointer to validate file existence */

    // Graph and lookup
    pGraph G;                                  /**< Pointer to the graph structure */
    int index;                                 /**< Index of the start vertex */

    // Output filenames
    char out1[120]; /**< Output file: vertex and edge set (-SET) */
    char out2[120]; /**< Output file: vertex degrees (-DEGREE) */
    char out3[120]; /**< Output file: adjacency list (-LIST) */
    char out4[120]; /**< Output file: adjacency matrix (-MATRIX) */
    char out5[120]; /**< Output file: BFS traversal (-BFS) */
    char out6[120]; /**< Output file: DFS traversal (-DFS) */

    // ==== Step 1: Prompt for Input File ====
    printf("Input filename: ");
    scanf("%s", inputFile);

    fileCheck = fopen(inputFile, "r");
    if (fileCheck == NULL) {
        printf("File %s not found.\n", inputFile);
        return 1;
    }
    fclose(fileCheck);

    // ==== Step 2: Build Graph ====
    G = gcreate(1);  // Initial dummy value; true size set by readInputFile
    if (!readInputFile(inputFile, G)) {
        freeGraph(G);
        return 1;
    }

    // ==== Step 3: Generate Basic Graph Output Files ====
    createOutputFileName(inputFile, "-SET", out1);      // Vertex and edge list
    createOutputFileName(inputFile, "-DEGREE", out2);   // Vertex degrees
    createOutputFileName(inputFile, "-LIST", out3);     // Adjacency list
    createOutputFileName(inputFile, "-MATRIX", out4);   // Adjacency matrix

    toTxt1VertexAndEdges(out1, G);
    toTxt2VertexDegrees(out2, G);
    toTxt3AdjList(out3, G);
    toTxt4AdjMatrix(out4, G);

    // ==== Step 4: Prompt for Traversal Start Vertex ====
    printf("Input start vertex for the traversal: ");
    scanf("%s", startVertex);

    // Check if the vertex exists in the graph
    if (findHead(G, startVertex, &index) == NULL) {
        printf("Vertex %s not found.\n", startVertex);
        freeGraph(G);
        return 1;
    }

    // ==== Step 5: Generate BFS and DFS Output Files ====
    createOutputFileName(inputFile, "-BFS", out5);  // BFS traversal output
    createOutputFileName(inputFile, "-DFS", out6);  // DFS traversal output

    toTxt5_BFS_Traversal(out5, G, startVertex);
    toTxt6_DFS_Traversal(out6, G, startVertex);

    // ==== Step 6: Free Memory ====
    freeGraph(G);

    return 0;
}
