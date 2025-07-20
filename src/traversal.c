// containing graph traversals
// #include "graph.c" -> for graph implementation 
#include "globals.h"
#include "bfs.c"
#include "dfs.c"
void toTxt5_BFS_Traversal(char out_filename[], pGraph GDS, char* start_vertex){
    /*
        G-BFS.TXT contains
        Clark Diana Hal Bruce
    */
   FILE* outFile;
   char(*visited)[MAX_ID_LEN+1]; //array containing the vertices with a maxiumum of 8 chars + \0
   visited = bfs(start_vertex, GDS); //storing the bfs result in visited
   //for debugging
//    for (int i = 0; visited!=NULL && i<GDS->nV; i++){
//         printf("%s. ", visited[i]);
//     }
    outFile = fopen(out_filename, "w");
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

