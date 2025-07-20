#include "graph.c"
#include "queue.c"

bool isVisited(char visited[][MAX_ID_LEN+1], int visitedCount, char* name){
    for(int i = 0; i<visitedCount;i++){
        if(strcmp(visited[i], name)==0){
            return true;
        }
    }
    return false;
}
char (*bfs(char* start, pGraph GDS))[MAX_ID_LEN+1]{
    char (*visited)[MAX_ID_LEN+1] = calloc(GDS->nV, sizeof(char[MAX_ID_LEN+1]));
    char (*temp)[MAX_ID_LEN+1] = calloc(GDS->nV, sizeof(char[MAX_ID_LEN+1]));
    char* current = "\0";
    pHead curHead;
    pHead tempHead;
    pVertex curVertex;
    int index;
    int visitedCount = 0;
    queue q;
    queue qtail;

    qnew(&q);

    nq(&q, &qtail, start);
    strcpy(visited[visitedCount], start);
    visitedCount++;

    while(visitedCount<4){
        dq(&q, current);

        curHead = findHead(GDS, current, &index);
        curVertex = curHead->list;

        while(curHead->list!=NULL){
            tempHead = findHead(GDS, curVertex->name, &index);
            if(tempHead!=NULL)
                strcpy(temp[index],curVertex->name);
            curVertex = curVertex->next;
        }
        for(int i = 0; i<GDS->nV; i++){
            if(strcmp(temp[i],"\0")!=0 && !isVisited(visited,visitedCount,temp[i])){
                nq(&q, &qtail, temp[i]);
            }
            free(temp[i]);
        }

    }

    return visited;
}

int main(){
    char** visited;
    pGraph graph1 = gcreate(4);
    pHead head1 = addHead(graph1, "Diana");
    addAdjacent(head1, "Hal");
    addAdjacent(head1, "Bruce");
    addAdjacent(head1, "Clark");
    pHead head2 = addHead(graph1, "Bruce");
    addAdjacent(head2, "Diana");
    pHead head3 = addHead(graph1, "Hal");
    addAdjacent(head3, "Clark");
    addAdjacent(head3, "Diana");
    pHead head4 = addHead(graph1, "Clark");
    addAdjacent(head4, "Hal");
    addAdjacent(head4, "Diana");
    
    // printGraph(graph1);
        for (int i = 0; i<4; i++){
            printf("%s. ", visited[i]);
        }
}