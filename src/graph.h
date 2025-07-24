#ifndef GRAPH
#define GRAPH

#include "globals.h"

/**
 * ALIST_VERTEX - names of the adjacent vertices of a specified head
 * 
 * next - next vertex connected to this vertex
 */
typedef struct ALIST_VERTEX{
    char name[MAX_ID_LEN+1];
    struct ALIST_VERTEX *next;
} *pVertex, Vertex;

/**
 * HEAD_VERTEX - first name in the adjacency list per line
 * list - the array of ALIST_VERTEX vertices of the head
 * list.next - next adjacent vertex of this head
 * nextHead - nextHead connected to current head
 */
typedef struct HEAD_VERTEX{
    char name[MAX_ID_LEN+1];
    pVertex list;
    struct HEAD_VERTEX *nextHead;
}*pHead, Head;

/**
 * UDGRAPH - undirected graph representation 
 * heads - list of heads 
 * nV - number of Vertices
 */
typedef struct UDGRAPH{
    int nV;
    pHead heads;
} *pGraph, UDGraph;

/**
 * Allocates space for a graph representation with V number of vertices
 * @author Kei Saguin
 * @param V number of vertices
 * @returns pointer to the graph representation
 */
pGraph gcreate(int V);

/**
 * Searches for the specified head name in the existing list of heads
 * in the graph
 * @author Kei Saguin
 * @param GDS Pointer to graph structure
 * @param name the name being searched
 * @param index pointer of the index of the name
 * @returns pointer to the head vertex containing the name being searched
 */
pHead findHead(pGraph GDS, char* name, int *index);

/**
 * Adds the specified head name to the tail of the existing list of heads
 * in the graph
 * @author Kei Saguin
 * @param GDS pointer to the graph structure
 * @param vertexName the vertex to be added to the heads list
 * @returns pointer to the head vertex being added
 */
pHead addHead(pGraph graph, char* vertexName);

/**
 * Searches for the specified adjacent name in the existing list of neighbors
 * of the head in the graph
 * @author Kei Saguin
 * @param headName Pointer to head vertex
 * @param name the name being searched
 * @returns pointer to the adjacent vertex containing the name being searched
 */
pVertex findAdjacent(pHead headName, char* name);

/**
 * Adds the specified neighbor name to the tail of the existing list of 
 * neighbors/adjacent vertices in the list of neighbors of the head specified
 * @author Kei Saguin
 * @param head pointer to the head vertex
 * @param neighbor the vertex to be added to the head's adjacent list
 */
void addAdjacent(pHead head, char* neighbor);

/**
 * Unallocates the space of the graph and clears it from memory
 * @param graph the pointer of the graph to clear
 */
void freeGraph(pGraph graph);

#endif