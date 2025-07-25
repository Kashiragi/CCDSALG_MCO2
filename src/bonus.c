#include "globals.h"
#include "graph.c"
#include "graphInfo.c"


void processBonus(pGraph g1, pGraph g2){
    // Variables
    pHead g1_h = g1->heads;
    pHead g2_h = g2->heads;
    int g1V = g1->nV, g2V = g2->nV; 
    pVertex adj;
    pEdge g1_e = malloc((g1V * (g1V-1))/2 * sizeof(Edge)); //n(n-1)/2
    pEdge g2_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    char (*final_v)[MAX_ID_LEN+1] = calloc(g2->nV, sizeof(char[MAX_ID_LEN+1]));
    pEdge final_e = malloc((g2V * (g2V-1))/2 * sizeof(Edge));
    int index, i = 0, e1count=0, e2count=0;
    int subvertices = 1, subedges = 1;

    // compare heads
    while(g2_h!=NULL && i<g2V){
        g1_h = findHead(g1,g2_h->name,&index);
        if(g1_h!=NULL){
            strcpy(final_v[i],g2_h->name); 
            strcat(final_v[i]," +"); 
            printf("%s\n", final_v[i]);
            
            i++;
        }
        else {
            strcpy(final_v[i],g2_h->name);  
            strcat(final_v[i]," -"); 
            printf("%s\n", final_v[i]);
            i++; 
            subvertices = 0; // automatically
        }
        g2_h = g2_h->nextHead;
    }
// qsort(vertice names);
    g1_h = g1->heads;
    g2_h = g2->heads;
// make edges of g1
    while(g1_h!=NULL){
        adj = g1_h->list;
        while(adj!=NULL){
            if(strcmp(g1_h->name,adj->name)<0){
                strcpy(g1_e[e1count].vertex1, g1_h->name);
                strcpy(g1_e[e1count].vertex2, adj->name);
                printf("(%s,%s)\n", g1_e[e1count].vertex1, g1_e[e1count].vertex2);
                e1count++;
            }
            adj = adj->next;
        }
        g1_h = g1_h->nextHead;
    }

    while(g2_h!=NULL){
        adj = g2_h->list;
        while(adj!=NULL){
            if(strcmp(g2_h->name,adj->name)<0){
                strcpy(g2_e[e2count].vertex1, g2_h->name);
                strcpy(g2_e[e2count].vertex2, adj->name);
                printf("(%s,%s)\n", g2_e[e2count].vertex1, g2_e[e2count].vertex2);
                e2count++;
            }
            adj = adj->next;
        }
        g2_h = g2_h->nextHead;
    }

    // comparing edges
    for(int k = 0, i = 0; i<e2count; i++){
        int found = 0;
        for(int j = 0; j<e1count; j++){
            if(strcmp(g2_e[i].vertex1,g1_e[j].vertex1)==0 &&
                strcmp(g2_e[i].vertex2,g1_e[j].vertex2)==0){
                    final_e[k] = g2_e[i];
                    found=1;
                    k++;
                    printf(" +\n");
                    break;
                }
        }
        if(!found){
            final_e[k] = g2_e[i];
            printf(" -\n");
            subedges = 0;
            k++;
        }
    }

}

int main(){
    pGraph g1 = gcreate(1), g2=gcreate(1);
    int read = readInputFile("G.txt",g1);
    read = readInputFile("H.txt",g2);

    processBonus(g1, g2);
}
