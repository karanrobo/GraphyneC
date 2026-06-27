#ifndef G_RENDER_H
#define G_RENDER_H

#include "../Graph/Graph.h"
#include <raylib.h>

/**
 * Api for the visualisation of the graph
 */
typedef struct graphV * GraphV;
typedef struct prop Prop;

typedef struct node *Adjnode; 

struct prop {
    Vector2 displacement;
    Vector2 velocity;
    Vector2 accleration;
};


struct graphV {
    Graph g;
    Prop *N;
};



typedef struct edgeV EdgeV;
typedef struct arrayOfEdgesV *AOEV;

// for visuals only
struct edgeV {
    float len;
};

struct arrayOfEdgesV {
    EdgeV **edges;
    int num_edges;
};



AOEV adlToAoe(GraphV gv);


void freeNN(Vector2 **arr, int nodes);
void drawNNrepEngine(Vector2 **arr, int *placement_arr, int nodes);
Vector2 **neuralNetworkRepEngine(int *placement_arr, int nodes, Rectangle dims, Vector2 padding);


GraphV test_graph(int num_nodes);

int graphRendered(GraphV gv, Vector2 offset, Vector2 start, float radius, int BALL_FONT);

#endif