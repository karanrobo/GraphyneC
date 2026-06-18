#include "Graph.h"

int main() {
    Graph new = newGraph(5);
    new = insertEdge(new, 0, 1);
    printGraph(new);
    new = insertEdge(new, 1, 2);
    printGraph(new);
    new = insertEdge(new, 2, 3);
    printGraph(new);
    new = insertEdge(new, 3, 4);
    printGraph(new);
    new = insertEdge(new, 4, 0);
    
    printGraph(new);


    freeGraph(new);
    return 0;
}