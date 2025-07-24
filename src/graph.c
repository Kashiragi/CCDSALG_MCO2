/**
 * @file graph.c
 * @details Contains functions and structs for the graph representation
 * which in this case is an adjacency list
 * @author VL Kirsten Camille Saguin
 */


#include "graph.h"

pGraph gcreate(int V){
    // Allocate the graph reference and assign the number of vertices
    pGraph graph = malloc(sizeof(UDGraph));
    if (graph == NULL) return NULL; 

    // set number of vertices of the graph
    assert(graph->nV = V);
    // Allocate the heads list reference and set to NULL (no heads yet)
    // assert(graph->heads = malloc(V*sizeof(Head*)));
    // set heads to NULL (no heads yet)
    graph->heads = NULL;

    return graph;
}

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
            // HEAD FOUND
            // index found at (since heads are added sequentially)
            *index = i;  // Set the value that index points to

            // current = the one with the name you are looking for
            return current;
        }
        // check the next head until all heads have been checked
        current = current->nextHead;
        // increment i since you are going to next head
        i++;
    }
    // HEAD NOT FOUND
    // index = -1 because not found
    *index = -1;
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
        pHead newHead = malloc(sizeof(Head));
        assert(strcpy(newHead->name, vertexName));
        // set nextHead and list of adjacent/neighbors of this head to NULL
        newHead->nextHead = NULL;
        newHead->list = NULL;

        // If the graph contains 0 HEADS at this time
        if(graph->heads == NULL) {
            graph->heads = newHead;
        }
        // If the graph contains at least 1 HEAD at this time
        else {
            // use c to traverse to the tail end of heads list in graph
            pHead c = graph->heads;
            // keep going to the nextHead until NULL
            while(c->nextHead!=NULL) c = c->nextHead;
            //add new head reference to the end of the heads list
            c->nextHead = newHead;
        }
        // return newHead reference that is added
        return newHead;
    }
    // return NULL, meaning the head with a specific name
    // that is trying to be added already exists in the graph
    return NULL;
}

pVertex findAdjacent(pHead headName, char* name){
    if(headName == NULL || name == NULL) return NULL;

    pVertex current = headName->list;
    
    // Preliminaries
    // Current vertex or adjacent vertex pointer must not be null
    // CONCEPT: searching is similar to linear search, as you go through each head at a time
    while(current!=NULL){
        // if the name on the current pHead is the same as the name in the param
        if(strcmp(current->name,name)==0){
            // HEAD FOUND
            // current = the one with the name you are looking for
            return current;
        }
        // check the next adjacent vertex until all adjacent of this head have been checked
        current = current->next;
    }
    // HEAD NOT FOUND
    return NULL;
    
    
}

void addAdjacent(pHead head, char* neighbor){
    if(head == NULL || neighbor == NULL) return;
    pVertex newVertex;
    pVertex c;
    // for clarity, adjacent is the same as neighbor
    // if neighbor name OF THIS HEAD doesn't exist in the graph yet, proceed
    // case sensitive so 
    // Juan=Juan (can't add) & Juan =/= JUAN (can add)
    if(findAdjacent(head,neighbor)==NULL){
        // Allocate new vertex reference and assign the name
        newVertex = malloc(sizeof(Vertex));
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
            c = head->list;
            // keep going to the next neighbor of this head until NULL
            while(c->next!=NULL) c = c->next;
            
            // add new vertex reference to the end of the list
            c->next = newVertex;
        }
    }
}

void freeGraph(pGraph graph) {
    if (graph == NULL) return;
    
    pHead current = graph->heads;
    pHead nextHead;
    pVertex nextVertex;
    pVertex vertex;
    
    while (current != NULL) {
        // store next head
        nextHead = current->nextHead;

        vertex = current->list;
        while (vertex != NULL) {
            // store next adjacent vertex of this head
            nextVertex = vertex->next;
            // free neighbor vertices of this head
            free(vertex);
            // move to next neighbor
            vertex = nextVertex;
        }
        // free the head itself
        free(current);
        // move to next neighbor
        current = nextHead;
    }
    // free graph itself
    free(graph);
}
