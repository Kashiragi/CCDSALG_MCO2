#include "graphInfo.c"
// #include "graph.c"

int main(){
    char* test = "heh";
    // pGraph graph1 = gcreate(4);
    // /**
    //  * Diana->Hal->Bruce->Clark->\
    //  Bruce->Diana->\
    //  Hal->Clark->Diana->\
    //  Clark->Hal->Diana->\
    //  */
    // pHead head1 = addHead(graph1, "Diana");
    // addAdjacent(head1, "Hal");
    // addAdjacent(head1, "Bruce");
    // addAdjacent(head1, "Clark");
    // pHead head2 = addHead(graph1, "Bruce");
    // addAdjacent(head2, "Diana");
    // pHead head3 = addHead(graph1, "Hal");
    // addAdjacent(head3, "Clark");
    // addAdjacent(head3, "Diana");
    // pHead head4 = addHead(graph1, "Clark");
    // addAdjacent(head4, "Hal");
    // addAdjacent(head4, "Diana");
    
    // printGraph(graph1);
    // toTxt4AdjMatrix(test, graph1);

    pGraph graph2 = gcreate(8);
    /**
     * 8
        A B E H -1
        B A C -1 
        C B G -1
        D E -1
        E A D G -1
        F G H -1
        G C E F H -1
        H A F G -1
     */
    pHead a = addHead(graph2, "A");
    addAdjacent(a, "B");
    addAdjacent(a, "E");
    addAdjacent(a, "H");
    pHead b = addHead(graph2, "B");
    addAdjacent(b, "A");
    addAdjacent(b, "C");
    pHead c = addHead(graph2, "C");
    addAdjacent(c, "B");
    addAdjacent(c, "G");
    pHead d = addHead(graph2, "D");
    addAdjacent(d, "E");
    pHead e = addHead(graph2, "E");
    addAdjacent(e, "A");
    addAdjacent(e, "D");
    addAdjacent(e, "G");
    pHead f = addHead(graph2, "F");
    addAdjacent(f, "G");
    addAdjacent(f, "H");
    pHead g = addHead(graph2, "G");
    addAdjacent(g, "C");
    addAdjacent(g, "E");
    addAdjacent(g, "F");
    addAdjacent(g, "H");
    pHead h = addHead(graph2, "H");
    addAdjacent(h, "A");
    addAdjacent(h, "F");
    addAdjacent(h, "G");

    printGraph(graph2);
    toTxt4AdjMatrix(test, graph2);

}