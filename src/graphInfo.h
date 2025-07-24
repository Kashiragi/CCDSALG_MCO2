#include "globals.h"

typedef struct EDGE {
    char vertex1[MAX_ID_LEN+1];
    char vertex2[MAX_ID_LEN+1];
}Edge, *pEdge;

/**
 *  Compares the value of two given strings. Helper function 
 *  used for qsort in toTxt1VertexAndEdges.
 *  
 *  @author Queenie Salao
 *  @param a address of the first string pointer
 *  @param b address of the second string pointer
 *  @return negative if *a < *b, positive if *a > *b, zero if equal
 *  
*/
int compareStrings(const void *a, const void *b);

/**
 *  Compares the value of two given strings. Helper function 
 *  used for qsort in toTxt1VertexAndEdges.
 *  
 *  @author Queenie Salao
 *  @param a address of the first edge pointer
 *  @param b address of the second edge pointer
 *  @return negative if *a < *b, positive if *a > *b, zero if equal
 */
int compareEdges(const void *a, const void *b);

/**
 *  Returns the number of vertices of the graph. 
 *  @author Queenie Salao
 *  @return the number of vertices of the graph 
 */
int countVertices(pGraph graph);

/**
 * Creates output file name by appending suffix to input filename.
 *
 * @author Queenie Salao
 * @param inputFileName Original filename (e.g. G.TXT)
 * @param suffix Suffix to add (e.g. -SET)
 * @param outputFileName Output filename with suffix and .TXT
 */
void createOutputFileName(char* inputFileName, char* suffix, char* outputFileName);

/**
 * Reads and parses a graph from a formatted text file.
 * Populates the graph structure with vertex and edge info.
 *
 * @author Queenie Salao
 * @param input_filename Name of the input .TXT file.
 * @param GDS Pointer to graph structure to fill.
 * @return 1 if successful, 0 otherwise.
*/
int readInputFile(char input_filename[], pGraph GDS);

/**
 *  Prints to a text file the vertices and edge, alphabetically, 
 *  based on the information from the graph.
 *  @param output_filename the file to output the result
 *  @param GDS Pointer to graph structure
*/
void toTxt1VertexAndEdges(char output_filename[], pGraph GDS);

/**
 *  Prints to a text file the vertices and edge, alphabetically, 
 *  based on the information from the graph.
 *  @param output_filename the file to output the result
 *  @param GDS Pointer to graph structure
 * */
void toTxt2VertexDegrees(char out_filename[], pGraph GDS);

/**
 *  Prints to a text file the adjacency list of the graph, 
 *  based on the information from the graph.
 *  @author Kurt Laguerta
 *  @param output_filename the file to output the result
 *  @param g Pointer to graph structure
 * */
void toTxt3AdjList(char out_filename[], pGraph GDS);

/**
 *  Takes the graph and converts it into an adjacency matrix (within this function only).
 *  Prints the matrix out to the text file specified
 * 
 *  @author VL Kirsten Camille D. Saguin
 *  @param out_filename the filename of the output file of the adjacency matrix
 *  @param g the graph structure containing information about the inputted graph
 * */
void toTxt4AdjMatrix(char out_filename[], pGraph g);