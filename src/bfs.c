#include "bfsutils.c"


char (*bfs(char* start, pGraph GDS))[MAX_ID_LEN+1]{
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

    if((curHead = findHead(GDS, start, &index))==NULL || GDS->heads==NULL){
        printf("Start doesn't exist???");
        return NULL;
    }

    qnew(&q);
    nq(&q, &qtail, start);
    // strcpy(visited[visitedCount], start);
    // printf("\nAdded to visited: %s\n", visited[visitedCount]);
    // dq(&q,temp[0]);
    // printf("\nAdded to dq: %s\n", temp[0]);

    
    while(q!=NULL && visitedCount<GDS->nV){
        printf("\nEntry bfs\n");
        dq(&q, current);
        printf("\nDeqd: %s\n", current);
        printf("visited count: %d\n", visitedCount);

        if(!isVisited(visited, visitedCount,current)){
            strcpy(visited[visitedCount], current);
            printf("\nAdded to visited: %s\n", visited[visitedCount]);
            visitedCount++;
            for(int i = 0; i<GDS->nV;i++){
                temp[i][0] = '\0';
            }
            
            curHead = findHead(GDS, current, &index);
            printf("\n!!Current head: %s\n", curHead->name);
            curVertex = curHead->list;

            while(curVertex!=NULL){
                tempHead = findHead(GDS, curVertex->name, &index);
                if(tempHead!=NULL){
                    strcpy(temp[index],curVertex->name);
                    printf("\n>>Current vertex added to temp: %s at index %d\n", temp[index], index);
                }
                curVertex = curVertex->next;
            }
            for(int i = 0; i<GDS->nV; i++){
                if(temp[i][0]!='\0' && !isVisited(visited,visitedCount,temp[i])){
                    nq(&q, &qtail, temp[i]);
                    printf("Enqd vertex: %s\n", temp[i]);
                }
            }
        
        }
        // printf("\nincrement: %d\n", visitedCount);
        // visitedCount++;
        // printf("\nincrement: %d\n", visitedCount);
    }

    return visited;
}

// int main(){
//     char(*visited)[MAX_ID_LEN+1];
//     pGraph graph1 = gcreate(4);
//     pHead head1 = addHead(graph1, "Diana");
//     addAdjacent(head1, "Hal");
//     addAdjacent(head1, "Bruce");
//     addAdjacent(head1, "Clark");
//     pHead head2 = addHead(graph1, "Bruce");
//     addAdjacent(head2, "Diana");
//     pHead head3 = addHead(graph1, "Hal");
//     addAdjacent(head3, "Clark");
//     addAdjacent(head3, "Diana");
//     pHead head4 = addHead(graph1, "Clark");
//     addAdjacent(head4, "Hal");
//     addAdjacent(head4, "Diana");
//     printGraph(graph1);
//     visited = bfs("Clark", graph1);
//     for (int i = 0; i<4; i++){
//         printf("%s. ", visited[i]);
//     }
// }