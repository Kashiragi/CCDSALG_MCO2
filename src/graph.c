//GDS = Graph Data Structure (either matrix or list)
// create gds here
// linked list
#include <string.h>
#define MAX_ID_LEN 8

typedef struct ALIST_VERTEX{
    char name[MAX_ID_LEN+1];
    struct ALIST_VERTEX *next;
} *pVertex;

typedef struct UDGRAPH{
    int nV;
    pVertex *list;
} *pGraph;

pVertex addVertex(char* vertexName){
    pVertex newVertex = malloc(sizeof(struct ALIST_VERTEX));
  
    assert(strncpy(newVertex->name, vertexName, MAX_ID_LEN));
    assert(newVertex->name[MAX_ID_LEN] = '\0');
    newVertex->next = NULL;

    return newVertex;
}

