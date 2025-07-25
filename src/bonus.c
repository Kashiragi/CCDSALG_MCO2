#include "globals.h"
#include "graph.c"
#include "graphInfo.c"

pEdge createEdges(pGraph g, int *edgeCount){
    pHead g1_h = g->heads;
    pVertex adj;
    int g1V = g->nV;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge));
    int e1count = 0;
    while(g1_h!=NULL){
        adj = g1_h->list;
        while(adj!=NULL){
            if(strcmp(g1_h->name,adj->name)<0){
                strcpy(g1_e[e1count].vertex1, g1_h->name);
                strcpy(g1_e[e1count].vertex2, adj->name);
                e1count++;
            }
            adj = adj->next;
        }
        g1_h = g1_h->nextHead;
    }
    *edgeCount = e1count;
    return g1_e;
}
void createBonusFilename(char* parentGraph, char* subGraph, char* suffix, char* outputFileName) {
    if (parentGraph == NULL || suffix == NULL || outputFileName == NULL) return;
    
    char* dotPos1 = strrchr(parentGraph, '.');
    char* dotPos2 = strrchr(subGraph, '.');
    if (dotPos1 != NULL) {
        int baseLength1 = dotPos1 - parentGraph;
        strncpy(outputFileName, parentGraph, baseLength1);
        outputFileName[baseLength1] = '\0';
        strcat(outputFileName,"-");
        int baseLength2 = dotPos2 - subGraph;
        strncat(outputFileName,subGraph,  baseLength2);
        outputFileName[baseLength1+baseLength2+1] = '\0';
        
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    } else {
        strcpy(outputFileName, parentGraph);
        strcat(outputFileName, "-");
        strcat(outputFileName, subGraph);
        strcat(outputFileName, suffix);
        strcat(outputFileName, ".TXT");
    }
}
void processBonus(char out_filename[],pGraph g1, pGraph g2){
    // Variables
    pHead g1_h = g1->heads;
    pHead g2_h = g2->heads;
    int g1V = g1->nV, g2V = g2->nV; 
    pVertex adj;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge)); //n(n-1)/2
    pEdge g2_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    char **final_v = malloc(g2V * sizeof(char[MAX_ID_LEN+1]));
    pEdge final_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    int index, i = 0, e1count=0, e2count=0;
    int subvertices = 1, subedges = 1;
    FILE* file;

    file = fopen(out_filename, "w");
    if (file==NULL){
        printf("nye");
        return;
    }

    // compare heads
    while(g2_h!=NULL && i<g2V){
        g1_h = findHead(g1,g2_h->name,&index);
        final_v[i] = malloc(sizeof(char)* (MAX_ID_LEN+1));

        if(g1_h!=NULL){
            strcpy(final_v[i],g2_h->name); 
            
            i++;
        }
        else {
            strcpy(final_v[i],g2_h->name);  
            i++; 
            subvertices = 0; // automatically
        }
        g2_h = g2_h->nextHead;
    }
    qsort(final_v, g2V, sizeof(char*), compareStrings);
    
    // make edges of g1
    g1_e = createEdges(g1, &e1count);
    // make edges of g2
    g2_e = createEdges(g2, &e2count);
    // comparing edges
    qsort(g2_e, e2count, sizeof(Edge), compareEdges);

    for(int k = 0, i = 0; i<e2count; i++){
        int found = 0;
        for(int j = 0; j<e1count; j++){
            if(strcmp(g2_e[i].vertex1,g1_e[j].vertex1)==0 && strcmp(g2_e[i].vertex2,g1_e[j].vertex2)==0){
                printf("(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
                printf(" +\n");
                found=1;
                k++;
                break;
            }
        }
        if(!found){
            printf("(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
            printf(" -\n");
            subedges = 0;
            k++;
        }
    }
    fclose(file);
}

int main(){
    pGraph g1 = gcreate(1), g2=gcreate(1);
    int read = readInputFile("G.txt",g1);
    read = readInputFile("K.txt",g2);
    char* out = NULL;
    createBonusFilename("G.txt", "K.txt", "-SUBGRAPH", out);
    printf("%s",out);
    processBonus(out,g1, g2);
}
