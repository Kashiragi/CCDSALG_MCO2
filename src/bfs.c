/**
 * @file bfs.c
 * @details Contains function for processing the BFS traversal (not .txt yet)
 * @author VL Kirsten Camille Saguin
 * @tester: Kurt Laguerta, Queenie Salao, Kei Saguin
 */
#include "bfsutils.c"

/**
 *  Goes through the graph's vertices and traverses it using Breadth First Search from the 
 *  designated starting vertex.
 *  @author Kei Saguin
 *  @param start the name of the starting vertex
 *  @param GDS the graph containing the information of vertices and connections
 *  @returns the char array containing the strings, in sequence, of the traversal from the start vertex
 */
char (*bfs(char* start, pGraph GDS))[MAX_ID_LEN+1]{
    // Variables
    char (*visited)[MAX_ID_LEN+1] = calloc(GDS->nV, sizeof(char[MAX_ID_LEN+1]));
    char (*temp)[MAX_ID_LEN+1] = calloc(GDS->nV, sizeof(char[MAX_ID_LEN+1]));
    char current[MAX_ID_LEN+1];
    pHead curHead;
    pHead tempHead;
    pVertex curVertex;
    int index;
    int visitedCount = 0;
    queue q;
    queue qtail;

    // if head doesn't exist in the graph
    if((curHead = findHead(GDS, start, &index))==NULL || GDS->heads==NULL) return NULL;

    // create a queue
    qnew(&q);
    // enqueue the start vertex
    nq(&q, &qtail, start);

    // continue the loop while the q is not empty and the visitedCount is not equal to the vertices count of the graph
    while(q!=NULL && visitedCount<GDS->nV)
    {
        // dequeue the head, transfer to current variable
        dq(&q, current);
        // if the current is not yet visited, proceed
        if(!isVisited(visited, visitedCount,current))
        {
            // copy the string in current to the array containing visted vertices
            strcpy(visited[visitedCount], current);
            visitedCount++;

            // initialize temp array with null byte
            for(int i = 0; i<GDS->nV;i++) { temp[i][0] = '\0';}
            
            // find the the vertex whose name matches current
            curHead = findHead(GDS, current, &index);

            curVertex = curHead->list;
            // continue until all neighbors or adjacent vertices are checked
            while(curVertex!=NULL)
            {
                // find curVertex's head counterpart and its index (sequence of heads during input)
                tempHead = findHead(GDS, curVertex->name, &index);

                if(tempHead!=NULL)
                    // add the name to the specified index in temp, to keep the head sequence
                    strcpy(temp[index],curVertex->name);

                // move on to the next adjacent vertex of this head
                curVertex = curVertex->next;
            }
            // go through all the vertices in temp
            for(int i = 0; i<GDS->nV; i++)
            {
                // if the index has a name and is not yet visited proceed
                if(temp[i][0]!='\0' && !isVisited(visited,visitedCount,temp[i]))
                    // enqueue the unvisited vertex
                    nq(&q, &qtail, temp[i]);
            }
        }
    }
    // Cleanup
    free(temp);
    qdestroy(&q);

    // return the array containing the visited vertices, in sequence
    return visited;
}