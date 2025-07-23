// containing graph traversals
// #include "graph.c" -> for graph implementation 
#include "globals.h"
#include "bfs.c"
#include "dfs.c"
/**
 *  Traverses the graph using BFS or Breadth First Search starting from the start vertex
 *  and outputting the traversal path to a text file
 *  @author VL Kirsten Camille Saguin
 *  @param out_filename the file to output the result to
 *  @param GDS the graph containing vertex and edge information
 *  @param start_vertex the string containing the starting vertex of the traversal
 */
void toTxt5_BFS_Traversal(char out_filename[], pGraph GDS, char* start_vertex){
    // Variable declarations
    FILE* outFile;
    char(*visited)[MAX_ID_LEN+1]; //array containing the vertices with a maxiumum of 8 chars + \0

    // call bfs() function, in turn storing the bfs result in visited
    visited = bfs(start_vertex, GDS); 
    
    outFile = fopen(out_filename, "w");
    // print the information contained in the visited array
    for (int i = 0; visited!=NULL && i<GDS->nV; i++){
        fprintf(outFile, "%s ", visited[i]);
    }
    fclose(outFile);
}

void toTxt6_DFS_Traversal(char out_filename[],pGraph GDS, char* start_vertex){
    /*
        G-BFS.TXT contains
        Clark Diana Bruce Hal
    */
}

