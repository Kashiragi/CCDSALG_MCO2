// contains code related to graph
// #include <string.h>
// #include "graph.c"
#include "globals.h"


// function to read, and process .txt file into graph representation (list or matrix)
// change void later to idk Graph return type so its reusable in main
void readInputFile(char input_filename[], pGraph GDS){
    //take the input
    //make into a graph representation
    
    /**
     * Pseudocode
     * 
     * DELIMETER_LEN 2*4
     * V = fscanf(first line)
     * arr[V+1]; 1D
     * line[8*V+1+DELIMETER_LEN]
     * while(fgets(line, sizeof(line),FILE)==V)
     *      firstToken = strtok(line, "->");
     *      nextToken;
     * 
     *      while(token!=null && flag){
     *          if strcmp(token,"\")==0
     *              break; or flag to leave while loop
     *          
     *          if(nextToken is empty)
     *              addNode(firstToken)
     *          else (firstToken!empty && nextToken!empty)
     *              addNode(nextToken)
     *              addEdge(firstToken, nextToken)
     * 
     *          nextToken = strtok(NULL, "->");
     * 
     *      }
     */
}

// first output file
void toTxt1VertexAndEdges(char output_filename[], pGraph GDS){
    /*
        G-SET.TXT contains:
        V(G)={Bruce,Clark,Diana,Hal}
        E(G)={(Bruce,Diana),(Clark,Diana),(Clark,Hal),(Diana,Hal)}
    */
}

void toTxt2VertexDegrees(char out_filename[], pGraph GDS){
    /*
        G-DEGREE.TXT contains
        Bruce 1
        Clark 2
        Diana 3
        Hal 2

    */
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
            printf("+%d+\n",j);
            if(targetHead!=NULL || j!=-1){
                adjMatrix[i][j] = 1;
                adjMatrix[j][i] = 1;
            }
            curVertex = curVertex->next;
            
        }
        curHead = curHead->nextHead;
        i++;
    }
    //for debugging of without text
        // pHead c = g->heads;
        // printf("  A B C D E F G H");
        // printf("\n");
        // for(int i=0; i<8;i++){
        //     printf("%s ", c->name);
        //     for(int j=0; j<8; j++){
        //         printf("%d ", adjMatrix[i][j]);
        //     }
        //     c = c->nextHead;
        //     printf("\n");
        // }

    //outputing to .txt file
    
}