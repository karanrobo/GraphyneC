/**
 * Api for the visualisation of the graph
 */

#ifndef G_RENDER_H
#define G_RENDER_H

#include "../Graph/Graph.h"
#include <raylib.h>

// Typedefs for moving the nodes in space
typedef struct graphV * GraphV;
typedef struct prop Prop;
typedef struct node *Adjnode; 

typedef struct edgeV EdgeV;
typedef struct arrayOfEdgesV *AOEV;

struct prop {
    Vector2 displacement;
    Vector2 velocity;
    Vector2 accleration;
};

struct graphV {
    Graph g;
    Prop *N;
};


// for visuals only
struct edgeV {
    float len;
};

struct arrayOfEdgesV {
    EdgeV **edges;
    int num_edges;
};



AOEV adlToAoe(GraphV gv);


Vector2 **neuralNetworkRepEngine(int *placement_arr, int nodes, Rectangle dims, Vector2 padding);
void drawNNrepEngine(Vector2 **arr, int *placement_arr, int nodes);
void freeNN(Vector2 **arr, int nodes);


GraphV test_graph(int num_nodes);

void graphLine(GraphV gv, Vector2 offset, Vector2 start, int BALL_FONT, int orientation);

void graphCircular(GraphV gv, Vector2 offset, int BALL_FONT, float auto_radius);

int graphBezierConnection(GraphV gv, Vector2 offset, Vector2 start, float radius, int BALL_FONT, int alternate);

int graphRendered(GraphV gv, Vector2 offset, Vector2 start, float radius, int BALL_FONT);

// update functions

/**
 * Api for updating the state of the graph
 * Calcualte forces, and update electrostatic/spring parameters
 */
void dragNode(GraphV gv, Vector2 mouse, float radius);

void updateElectrostaticSpringForce(GraphV gv, AOEV av, float C, float k);

void applyDrag(GraphV gv, float dt, float drag_constant) ;

void updateLoopIntegral(GraphV gv, float dt);


#endif