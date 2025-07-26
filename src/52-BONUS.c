/**
 * @file 52-BONUS.c
 * @description Contains program to run the bonus task: checking whether the second
 * graph is a subgraph of the first.
 * @note calls functions from graph.c and graphInfo.c
 * @author VL Kirsten Camille Saguin
 * @tester Kurt Laguerta, Queenie Salao, Kei Saguin
 */
#include "globals.h"
#include "graph.c"
#include "graphInfo.c"
/**
 *  Creates the edges of a given graph, with the vertices being alphabetically arranged
 *  @author Kei Saguin
 *  @param g the graph structure containing the adjacent list representation
 *  @param edgeCount the number of edges of the graph
 *  @return pointer to the collection of edges of this graph
 */
pEdge createEdges(pGraph g, int *edgeCount){
    // Variable declaration
    int g1V = g->nV,
        e1count = 0;
    pHead g1_h = g->heads;
    pVertex adj;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge));

    // Check all heads of this graph
    while(g1_h!=NULL){
        adj = g1_h->list;
        // Check all adjacent vertices or neighbors of this head
        while(adj!=NULL){
            if(strcmp(g1_h->name,adj->name)<0){
                strcpy(g1_e[e1count].vertex1, g1_h->name);
                strcpy(g1_e[e1count].vertex2, adj->name);
                e1count++;
            }
            // Move on to next adjacent vertex
            adj = adj->next;
        }
        // Move on to next head in graph
        g1_h = g1_h->nextHead;
    }
    // return
    *edgeCount = e1count;
    return g1_e;
}
/**
 *  Creates the filename for the subgraph text file, based on the graph names of the first and second graphs.
 *  @author Kei Saguin
 *  @param parentGraph the first graph or the graph to compare to
 *  @param subGraph the second graph or the graph being checked
 *  @param suffix Suffix to add (e.g. -SUBGRAPH)
 *  @param outputFilename Output filename with suffix and .TXT
 */
void createBonusFilename(char* parentGraph, char* subGraph, char* suffix, char* outputFileName) {
    if (parentGraph == NULL || suffix == NULL || outputFileName == NULL) return;
    // Get positions of the . (start of fiel extension)
    char* dotPos1 = strrchr(parentGraph, '.');
    char* dotPos2 = strrchr(subGraph, '.');
    if (dotPos1 != NULL) {
        int baseLength1 = dotPos1 - parentGraph;
        int baseLength2 = dotPos2 - subGraph;
        // copy the filename based on the baseLength
        strncpy(outputFileName, parentGraph, baseLength1);
        outputFileName[baseLength1] = '\0';
        strcat(outputFileName,"-");
        strncat(outputFileName,subGraph,  baseLength2);
        
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    } else {
        strcpy(outputFileName, parentGraph);
        strcat(outputFileName, "-");
        strcat(outputFileName, subGraph);
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    }
}
/**
 *  Checks if the second graph is a subgraph of the first graph.
 *  @author Kei Saguin
 *  @param out_filename the file to print the output to
 *  @param g1 the first graph
 *  @param g2 the second graph
 */
void checkSubgraph(char out_filename[],pGraph g1, pGraph g2){
    // Variables
    pHead g1_h = g1->heads, 
          g2_h = g2->heads;
    int index, 
        g1V = g1->nV, g2V = g2->nV, 
        i = 0, 
        e1count=0, e2count=0, 
        subvertices = 1, subedges = 1;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge)), //n(n-1)/2
          g2_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    char **final_v = malloc(g2V * sizeof(char*));
    FILE* file;


    file = fopen(out_filename, "w");
    if (file==NULL){
        return;
    }

    // compare heads
    while(g2_h!=NULL && i<g2V){
        // Check if the head exists in the graph
        g1_h = findHead(g1,g2_h->name,&index);
        final_v[i] = malloc(sizeof(char) * (MAX_ID_LEN+1));

        // if exists, +
        if(g1_h!=NULL){
            strcpy(final_v[i],g2_h->name);
            strcat(final_v[i]," +");
            i++;
        }
        // if doesn't exist, -
        else {
            strcpy(final_v[i],g2_h->name);  
            strcat(final_v[i]," -");
            i++; 
            // since not part of vertices, set 0
            subvertices = 0; 
        }
        // move on to next head
        g2_h = g2_h->nextHead;
    }

    // Sort the strings alphabetically
    qsort(final_v, g2V, sizeof(char*), compareStrings);

    // Print the heads and their symbols (if incl. or not)
    for(i=0; i<g2V; i++){
        fprintf(file, "%s\n", final_v[i]);
    }

    // Make edges of graph 1
    g1_e = createEdges(g1, &e1count);
    // Make edges of graph 2
    g2_e = createEdges(g2, &e2count);

    // Sort the edges alphabetically
    qsort(g2_e, e2count, sizeof(Edge), compareEdges);

    // Process edges
    for(int k = 0, i = 0; i<e2count; i++){
        int found = 0;
        // Compare if edges exist in graph 1's edges
        for(int j = 0; j<e1count; j++){
            if(strcmp(g2_e[i].vertex1,g1_e[j].vertex1)==0 && strcmp(g2_e[i].vertex2,g1_e[j].vertex2)==0){
                // Mark included
                fprintf(file, "(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
                fprintf(file, " +\n");
                // Mark found
                found=1;
                k++;
                // Leave inner loop
                break;
            }
        }
        // if not found, not even once
        if(!found){
            // Mark excluded
            fprintf(file, "(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
            fprintf(file, " -\n");
            subedges = 0; // auto 0
            k++;
        }
    }

    // Print conclusion
    if(subvertices!=0 && subedges !=0) fprintf(file, "%c is a subgraph of %c.", out_filename[2], out_filename[0]);
    else fprintf(file, "%c is not a subgraph of %c.", out_filename[2], out_filename[0]);
    
    // Cleanup
    free(g1_e);
    free(g2_e);
    for(int j = 0; j < g2V; j++) {
        free(final_v[j]);
    }
    free(final_v);

    fclose(file);
}
/**
 * Main entry point of the program.
 * Loads 2 undirected graphs from their respective files, and generates
 * the vertices and edges that included and not included in a text file.
 *
 * @author  VL Kirsten Camille Saguin
 * @param   none
 * @return  int (0 on success, 1 on error)
 * @tester Kurt Laguerta, Queenie Salao, Kei Saguin
 */
int main(){
    // ==== Variable Declarations
    pGraph g1, g2;                                      /** Graph representation */
    FILE *fileCheck1, *fileCheck2;                      /** File pointer to validate file existence */
    char firstGraph[120], secondGraph[120],             /** Input filenames for the two files */
         out7[120];                                     /** Output file: Subgraph */

    // ==== Prompt input files ====
    printf("Input first graph: ");
    scanf("%s", firstGraph);
    printf("Input second graph: ");
    scanf("%s", secondGraph);

    // ==== Check if files DO exist ====
    fileCheck1 = fopen(firstGraph, "r");
    fileCheck2 = fopen(secondGraph, "r");
    
    if (fileCheck1 == NULL || fileCheck2 == NULL) {
        printf("Files not found.\n");
        fclose(fileCheck1);
        fclose(fileCheck2);
        return 1;
    }
    // ==== Create graphs. 1 is initial dummy value. true size will be set after reading file
    g1 = gcreate(1);
    g2 = gcreate(1);
    if (!readInputFile(firstGraph, g1) || !readInputFile(secondGraph, g2)) {
        freeGraph(g1);
        freeGraph(g2);
        return 1;
    }

    // ==== Create Filename for bonus e.g. First-Second-SUBGRAPH
    createBonusFilename(firstGraph, secondGraph, "-SUBGRAPH", out7);

    // === Checking if second graph is a subgraph of the first graph ===
    checkSubgraph(out7, g1, g2);

    // Cleanup
    freeGraph(g1);
    freeGraph(g2);

    return 0;
}
