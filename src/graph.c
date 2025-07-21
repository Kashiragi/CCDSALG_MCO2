/**
 * @author Saguin
 * @checker pls check
 */

#ifndef GRAPH
#define GRAPH
//GDS = Graph Data Structure (either matrix or list)
// create gds here
// linked list
// #include <string.h>
// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>
// #define MAX_ID_LEN 8
#include "globals.h"

/**
 * HEAD_VERTEX - first name in the adjacency list per line
 * list is the array of adj vertices of the head
 * next - next neighbor connected to current head
 */
typedef struct ALIST_VERTEX{
    char name[MAX_ID_LEN+1];
    struct ALIST_VERTEX *next;
} *pVertex;

/**
 * HEAD_VERTEX - first name in the adjacency list per line
 * list is the array of adj vertices of the head
 * nextHead - nextHead connected to current head
 */
typedef struct HEAD_VERTEX{
    char name[MAX_ID_LEN+1];
    pVertex list;
    struct HEAD_VERTEX *nextHead;
}*pHead;
/**
 * heads - list of heads 
 * nV - number of Vertices
 */
typedef struct UDGRAPH{
    int nV;
    pHead heads;
} *pGraph;


pHead findHead(pGraph GDS, char* name, int *index){
    //initial checking of values entered into the function
    if (GDS == NULL || name == NULL) return NULL;
    
    pGraph g = GDS;
    int i = 0;
    pHead current = g->heads;
    // Preliminaries
    // Current head pointer must not be null
    // CONCEPT: searching is similar to linear search, as you go through each head at a time
    while(current!=NULL){
        // if the name on the current pHead is the same as the name in the param
        if(strcmp(current->name,name)==0){
            // head found in list
            printf("Head found: %s", current->name);
            // index found at (since heads are added sequentially)
            *index = i;  // Set the value that index points to
            printf(": %d\n",*index);
            // current = the one with the name you are looking for
            return current;
        }
        // check the next head until all heads have been checked
        current = current->nextHead;
        // increment i since you are going to next head
        i++;
    }
    // head not found in list
    printf("\nHead in this graph with name \"%s\" not found", name);
    // index = -1 because not found
    *index = -1;
    printf(": %d\n",*index);
    return NULL;

    
}

pHead addHead(pGraph graph, char* vertexName){
    if (graph == NULL || vertexName == NULL) return NULL;

    int index;
    // if head name doesn't exist in the graph yet, proceed
    // case sensitive so 
    // Juan=Juan (can't add) & Juan =/= JUAN (can add)
    // for this checking purpose, index is not used and is only a placeholder
    if(findHead(graph, vertexName, &index)==NULL){
        // Allocate a new head reference and assign the name
        pHead newHead = malloc(sizeof(struct HEAD_VERTEX));
        assert(strcpy(newHead->name, vertexName));
        // set nextHead and list of adjacent to NULL
        newHead->nextHead = NULL;
        newHead->list = NULL;

        // printf("HEAD added: %s", newHead->name);
        // If the graph contains 0 HEADS at this time
        if(graph->heads == NULL) {
            graph->heads = newHead;
            printf("Head added");
        }
        // If the graph contains >1 HEAD at this time
        else{
            // use c to traverse to the tail end of heads list in graph
            pHead c = graph->heads;
            // keep going to the nextHead until NULL
            while(c->nextHead!=NULL) c = c->nextHead;
            
            //add new head reference to the end of the heads list
            c->nextHead = newHead;
            printf("Head added");
        }
        // printf("%s:", newHead->name);
        // return newHead reference that is added
        return newHead;
    }
    // return NULL, meaning the head with a specific name
    // that is trying to be added already exists in the graph
    return NULL;
}

/**
 * V - number of vertices
 */
pGraph gcreate(int V){
    // Allocate the graph reference and assign the number of vertices
    pGraph graph = malloc(sizeof(struct UDGRAPH));
    if (graph == NULL) return NULL; 

    assert(graph->nV = V);
    // Allocate the heads list reference and set to NULL (no heads yet)
    assert(graph->heads = malloc(V*sizeof(struct ALIST_VERTEX*)));
    graph->heads = NULL;

    printf("Graph created");
    return graph;
}
/**
 * This function looks for a specific neighbor or adjacent vertex 
 * of a SPECIFIC HEAD. Adjacent of THIS HEAD only.
 */
pVertex findAdjacent(pHead headName, char* name){
    if(headName == NULL || name == NULL) return NULL;

    pVertex current = headName->list;
    
    // Preliminaries
    // Current vertex or adjacent vertex pointer must not be null
    // CONCEPT: searching is similar to linear search, as you go through each head at a time
    while(current!=NULL){
        // if the name on the current pHead is the same as the name in the param
        if(strcmp(current->name,name)==0){
            // head found in list
            printf("Neighbor found: %s", current->name);
            // current = the one with the name you are looking for
            return current;
        }
        // check the next adjacent vertex until all adjacent of this head have been checked
        current = current->next;
    }
    // head not found in this list
    printf("Neighbor in this head with name \"%s\" not found", name);
    return NULL;
    
    
}
void addAdjacent(pHead head, char* neighbor){
    if(head == NULL || neighbor == NULL) return;

    // for comments sake, adjacent is the same as neighbor
    // if neighbor name OF THIS HEAD doesn't exist in the graph yet, proceed
    // case sensitive so 
    // Juan=Juan (can't add) & Juan =/= JUAN (can add)
    if(findAdjacent(head,neighbor)==NULL){
        // Allocate new vertex reference and assign the name
        pVertex newVertex = malloc(sizeof(struct ALIST_VERTEX));
        if (newVertex == NULL) return; 

        assert(strcpy(newVertex->name, neighbor));
        // set next to NULL
        newVertex->next = NULL;

        // If the head contains 0 NEIGHBORS at this time
        if(head->list == NULL) 
            head->list = newVertex;
        // If the head contains at leatt 1 NEIGHBOR at this time
        else{
            // use c to traverse to the tail end of the head's neighbor list in graph
            pVertex c = head->list;
            // keep going to the next neighbor of this head until NULL
            while(c->next!=NULL) c = c->next;
            
            // add new vertex reference to the end of the list
            c->next = newVertex;
        }
        printf("Adj created");
    }
}
// actually, this is the code for list so copy past nlng lmao
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

#endif