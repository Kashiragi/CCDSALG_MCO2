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
        int baseLength2 = dotPos2 - subGraph;
        strncpy(outputFileName, parentGraph, baseLength1);
        outputFileName[baseLength1] = '\0';

        strcat(outputFileName,"-");
        strncat(outputFileName,subGraph,  baseLength2);
        
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
void checkSubgraph(char out_filename[],pGraph g1, pGraph g2){
    // Variables
    pHead g1_h = g1->heads, 
          g2_h = g2->heads;
    int index, 
        g1V = g1->nV, g2V = g2->nV, 
        i = 0, 
        e1count=0, e2count=0, 
        subvertices = 1, subedges = 1;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge)), //n(n-1)/2
          g2_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    char **final_v = malloc(g2V * sizeof(char[MAX_ID_LEN+1]));
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
            strcat(final_v[i]," +");
            i++;
        }
        else {
            strcpy(final_v[i],g2_h->name);  
            strcat(final_v[i]," -");
            i++; 
            subvertices = 0; // automatically
        }
        g2_h = g2_h->nextHead;
    }

    qsort(final_v, g2V, sizeof(char*), compareStrings);

    for(i=0; i<g2V; i++){
        fprintf(file, "%s\n", final_v[i]);
    }

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
                fprintf(file, "(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
                fprintf(file, " +\n");
                found=1;
                k++;
                break;
            }
        }
        if(!found){
            fprintf(file, "(%s,%s)",g2_e[i].vertex1, g2_e[i].vertex2);
            fprintf(file, " -\n");
            subedges = 0;
            k++;
        }
    }

    if(subvertices!=0 && subedges !=0) fprintf(file, "%c is a subgraph of %c.", out_filename[2], out_filename[0]);
    else fprintf(file, "%c is not a subgraph of %c.", out_filename[2], out_filename[0]);
    
    free(g1_e);
    free(g2_e);
    
    free(final_v);
    fclose(file);
}

int main(){
    pGraph g1, g2;
    FILE *fileCheck1, *fileCheck2;
    char firstGraph[120], secondGraph[120], out7[120];

    // ==== Prompt input files ====
    printf("Input first graph: ");
    scanf("%s", firstGraph);
    printf("Input second graph: ");
    scanf("%s", secondGraph);

    // ==== Check if files DO exist ====
    fileCheck1 = fopen(firstGraph, "r");
    fileCheck2 = fopen(secondGraph, "r");
    
    if (fileCheck1 == NULL || fileCheck2 == NULL) {
        printf("Files not found.\n");
        fclose(fileCheck1);
        fclose(fileCheck2);
        return 1;
    }
    // ==== Create graphs. 1 is initial dummy value. true size will be set after reading file
    g1 = gcreate(1);
    g2 = gcreate(1);
    if (!readInputFile(firstGraph, g1) || !readInputFile(secondGraph, g2)) {
        freeGraph(g1);
        freeGraph(g2);
        return 1;
    }

    // ==== Create Filename for bonus e.g. First-Second-SUBGRAPH
    createBonusFilename(firstGraph, secondGraph, "-SUBGRAPH", out7);

    // === Checking if second graph is a subgraph of the first graph ===
    checkSubgraph(out7, g1, g2);

    // Cleanup
    freeGraph(g1);
    freeGraph(g2);

    return 0;
}
