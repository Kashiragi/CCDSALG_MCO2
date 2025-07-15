// contains code related to graph
// #include <string.h>
// #include "graph.c"


// function to read, and process .txt file into graph representation (list or matrix)
// change void later to idk Graph return type so its reusable in main
void readInputFile(char input_filename[], GDS){
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
void toTxt1VertexAndEdges(char output_filename[], GDS){
    /*
        G-SET.TXT contains:
        V(G)={Bruce,Clark,Diana,Hal}
        E(G)={(Bruce,Diana),(Clark,Diana),(Clark,Hal),(Diana,Hal)}
    */
}

void toTxt2VertexDegrees(char out_filename[], GDS){
    /*
        G-DEGREE.TXT contains
        Bruce 1
        Clark 2
        Diana 3
        Hal 2

    */
}

void toTxt3AdjList(char out_filename[], GDS){
    /*
        G-LIST.TXT contains
        Diana->Hal->Bruce->Clark->\
        Bruce->Diana->\
        Hal->Clark->Diana->\
        Clark->Hal->Diana->\
    */

}

void toTxt4AdjMatrix(char out_filename[], GDS){
    /*
        G-MATRIX.TXT contains
                Diana Bruce Hal Clark
        Diana   0     1     1   1
        Bruce   1     0     0   0
        Hal     1     0     0   1
        Clark   1     0     1   0
    */
}