//GDS = Graph Data Structure (either matrix or list)
// create gds here
// linked list
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_ID_LEN 8

typedef struct ALIST_VERTEX{
    char name[MAX_ID_LEN+1];
    struct ALIST_VERTEX *next;
} *pVertex;

/**
 * list is the array of adj vertices of the head
 */
typedef struct HEAD_VERTEX{
    char name[MAX_ID_LEN+1];
    pVertex list;
    struct HEAD_VERTEX *nextHead;
}*pHead;
typedef struct UDGRAPH{
    int nV;
    pHead heads;
} *pGraph;

pHead addHead(pGraph graph, char* vertexName){
    pHead newHead = malloc(sizeof(struct HEAD_VERTEX));
  
    assert(strcpy(newHead->name, vertexName));
    // assert(newHead->name[strlen(newHead->name)-1] = '\0');
    newHead->nextHead = NULL;
    newHead->list = NULL;

    // printf("HEAD added: %s", newHead->name);

    if(graph->heads == NULL) {
        graph->heads = newHead;
        printf("Head added");
    }

    else{
        pHead c = graph->heads;
        while(c->nextHead!=NULL) c = c->nextHead;
        
        c->nextHead = newHead;
    }
    // printf("%s:", newHead->name);

    return newHead;
}

pGraph gcreate(int V){
    pGraph graph = malloc(sizeof(struct UDGRAPH));
    assert(graph->nV = V);
    assert(graph->heads = malloc(V*sizeof(struct ALIST_VERTEX*)));

    graph->heads = NULL;

    printf("Graph created");
    return graph;
}

void addAdjacent(pHead head, char* neighbor){
    pVertex newVertex = malloc(sizeof(struct ALIST_VERTEX));
    assert(strcpy(newVertex->name, neighbor));
    // assert(newVertex->name[MAX_ID_LEN] = '\0');
    newVertex->next = NULL;

    if(head->list == NULL) head->list = newVertex;
    else{
        pVertex c = head->list;
        while(c->next!=NULL) c = c->next;
        
        c->next = newVertex;
    }
    printf("Adj created");
}

pHead findHead(pGraph GDS, char* name){
    pHead current = GDS->heads;
    int flag = 1;
    while(strcmp(current->name,name)!=0 && flag){
        current = current->nextHead;

        if(current == NULL) {
            flag=0;
            printf("Head not found");
        }
    }

    printf("%s", current->name);
    return current;
    
    
}

void printGraph(pGraph graph){
    pHead curhead = graph->heads;
    pVertex curvertex = curhead->list;
    while(curhead!=NULL){
        printf("test");
        // printf("~~%s ->", curhead->name);
        while(curvertex!=NULL){
            // printf("| %s->", curvertex->name);
            curvertex = curvertex->next;
        }
        curhead = curhead->nextHead;
    }
}

int main(){

    pGraph graph = gcreate(4);
    pHead head = addHead(graph, "Diana");
    printf("HEAD IN MAIND: %s", head->name);
    addAdjacent(head, "Kurt");

    printGraph(graph);

}