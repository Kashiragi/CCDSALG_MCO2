/**
 * @author Saguin
 * @checker pls check
 */

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

pHead findHead(pGraph GDS, char* name, int *index){
    pGraph g = GDS;
    int i = 0;
    pHead current = g->heads;
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            printf("Head found: %s", current->name);
            *index = i;  // Set the value that index points to
            return current;
        }
        current = current->nextHead;
        i++;
    }
    printf("Head in this graph with name \"%s\" not found", name);
    *index = -1;
    return NULL;

    
}

pHead addHead(pGraph graph, char* vertexName){
    int index;
    if(findHead(graph, vertexName, &index)==NULL){
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
            printf("Head added");
        }
        // printf("%s:", newHead->name);

        return newHead;
    }
    return NULL;
}

pGraph gcreate(int V){
    pGraph graph = malloc(sizeof(struct UDGRAPH));
    assert(graph->nV = V);
    assert(graph->heads = malloc(V*sizeof(struct ALIST_VERTEX*)));

    graph->heads = NULL;

    printf("Graph created");
    return graph;
}
pVertex findAdjacent(pHead headName, char* name){
    pHead h = headName;
    pVertex current = h->list;
    
    while(current!=NULL){
        if(strcmp(current->name,name)==0){
            printf("Neighbor found: %s", current->name);
            return current;
        }
        current = current->next;
    }
    printf("Neighbor in this head with name \"%s\" not found", name);
    return NULL;
    
    
}
void addAdjacent(pHead head, char* neighbor){
    if(findAdjacent(head,neighbor)==NULL){
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
}





void printGraph(pGraph graph){

    pHead curhead = graph->heads;
    printf("loe\n");
    while(curhead!=NULL){
        pVertex curvertex = curhead->list;
        printf("test");
        printf("~~%s ->", curhead->name);
        while(curvertex!=NULL){
            printf("| %s->", curvertex->name);
            curvertex = curvertex->next;
        }
        printf("\\\n");

        curhead = curhead->nextHead;
    }
}

// int main(){
//     int index;
//     pGraph graph = gcreate(4);
//     pHead head = addHead(graph, "Diana");
//     pHead head2 = addHead(graph, "Bruce");
//     pHead head3 = findHead(graph, "Bruce", &index);
//     printf("\n~~~~%d~~~~\n",index);
//     addAdjacent(head, "Hal");
//     addAdjacent(head, "Bruce");
//     addAdjacent(head, "Clark");
//     addAdjacent(head2, "Diana");
//     addAdjacent(head2, "Diana");

//     printGraph(graph);

// }