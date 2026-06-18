#include "Graph.h"
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


AOEV adlToAoe(GraphV gv) {
    AOEV av = malloc(sizeof(struct arrayOfEdgesV));
    int verts = gv->g->vertices;

    int edges = 0;
    for (int i = 0; i < gv->g->vertices; i++) {
        Adjnode cn = gv->g->v[i];
        for (; cn != NULL; cn = cn->next) {
            edges++;
        }
    }
    
    av->num_edges = edges;

    EdgeV **ed = malloc(sizeof(struct edgeV *) * verts);
    // not the edge lenght but initial lengths
    for (int i = 0; i < verts; i++) {
        ed[i] = malloc(sizeof(struct edgeV) * verts);

        for (int j = 0; j < verts; j++) {
            if (i != j) {
                if (containsEdge(gv->g, i, j)) {
                    float dx = gv->N[i].displacement.x - gv->N[j].displacement.x;
                    float dy = gv->N[i].displacement.y - gv->N[j].displacement.y;
                    
                    ed[i][j].len = sqrtf(dx*dx + dy*dy);
                } else {
                    ed[i][j].len = -1;
                }
            }
                                                                         
        }
    }

    av->edges = ed;

    return av;
}



void freeNN(Vector2 **arr, int nodes) {
    for (int i = 0; i < nodes; i++) {
        free(arr[i]);
    }
    free(arr);
}

void drawNNrepEngine(Vector2 **arr, int *placement_arr, int nodes) {
    for (int i = 0; i < nodes - 1; i++) {
        int plen = placement_arr[i];
        int plenp = placement_arr[i+1];
        for (int j = 0; j < plen; j++) {
            //DrawCircleLines(arr[i][j].x, arr[i][j].y, 5, RED);
            for (int k = 0; k < plenp; k++) {
                DrawLine(arr[i][j].x, arr[i][j].y, arr[i+1][k].x, arr[i+1][k].y, RED);
            }
        }
    }
}


// arrange the graph in a neural network like formation 
// we also take another array which shows how many nodes in single, but how to connect, does note care abou the graph 
Vector2 **neuralNetworkRepEngine(int *placement_arr, int nodes, Rectangle dims, Vector2 padding) {
    // tries to place the N nodes as per the placement_arr, or does the best it can
    // if (padding.x > dims.width * 0.5 || padding.y > dims.height * 0.5) return; // padding is too large
    DrawRectangleLines(dims.x, dims.y, dims.width, dims.height, RED);
    Rectangle fin = (Rectangle) {
        .x = dims.x + padding.x, 
        .y = dims.y + padding.y, 
        .height = dims.height - 2*padding.y, 
        .width = dims.width - 2*padding.x
    };

    DrawRectangleLines(fin.x, fin.y, fin.width, fin.height, RED);

    float x_div = fin.width / (nodes - 1);
    float xcurr = fin.x;
    

    Vector2 **arr = malloc(sizeof(*arr) * nodes);
    for (int i = 0; i < nodes; i++) {
        int plen = placement_arr[i];
        float y_div = fin.height / plen; 
        arr[i] = malloc(sizeof(Vector2) * plen); 
        float ycurr = fin.y + y_div/2;
        for (int j = 0; j < plen; j++) {
            arr[i][j] = (Vector2){xcurr,ycurr};
            ycurr += y_div;
        }
        xcurr += x_div; 
    }

    return arr;
}


/*
Need algorithm that placed the graph nodes so that the intersection is minimised 
Or go element wise making connections 
    Find each node placement Vector2 w, such that they repel 

    the line is fixed and provides opposing force 
    each ball repells every other ball with force 1/r^2 * p
*/

#define BALL_FONT 20

GraphV test_graph() {

    int num_nodes = 6;

    GraphV gv = malloc(sizeof(struct graphV));
    gv->g = newGraph(num_nodes);
    gv->N = malloc(sizeof(Prop) * (gv->g->vertices));

    

    // representation scheme
    // NN, circular, adaptive(electromagnetic repulsion)
    for (int i = 0; i < gv->g->vertices; i++) {
        gv->N[i] = (Prop){.displacement = (Vector2){GetRandomValue(100, 500),GetRandomValue(100, 500)}, (Vector2){0,0}, (Vector2){0,0}};
        //.velocity = {GetRandomValue(-10,10),GetRandomValue(-10,10)}, .accleration = {GetRandomValue(-10,10),GetRandomValue(-10,10)}};
    }

   

    // for spacing out: (Vector2){i*150 + 100,GetRandomValue(300, 400)}
    gv->g = insertEdge(gv->g, 0, 1);
    // printGraph(gv->g);
    gv->g = insertEdge(gv->g, 1, 2);
    // printGraph(gv->g);
    gv->g = insertEdge(gv->g, 2, 3);
    // printGraph(gv->g);
    gv->g = insertEdge(gv->g, 3, 4);
    // printGraph(gv->g);
    //gv->g = insertEdge(gv->g, 4, 0);

    gv->g = insertEdge(gv->g, 4, 2);

    gv->g = insertEdge(gv->g, 2, 4);

    
    gv->g = insertEdge(gv->g, 0, 3);
    gv->g = insertEdge(gv->g, 1, 4);
    gv->g = insertEdge(gv->g, 4, 0);
    gv->g = insertEdge(gv->g, 3, 1);
    gv->g = insertEdge(gv->g, 5, 2);
    gv->g = insertEdge(gv->g, 5, 1);

    // for (int i = 0; i < num_nodes * 2; i++) {
    //     gv->g = insertEdge(gv->g, GetRandomValue(0,num_nodes), GetRandomValue(0,num_nodes));
    // }
    
    // printGraph(n);
    return gv;
}


int graphRendered(GraphV gv, Vector2 offset, Vector2 start, float radius) {
    for (int i = 0; i < gv->g->vertices; i++) {
        //gv->N[i] = (Vector2) {gv->N[i].x + start.x*i + offset.x , gv->N[i].y + start.y + offset.y};
        Prop curr = gv->N[i];
        DrawCircle(curr.displacement.x, curr.displacement.y, radius, BLACK);
        DrawText(TextFormat("%d", i), curr.displacement.x - BALL_FONT/2, curr.displacement.y - BALL_FONT/2, BALL_FONT, WHITE);
       
        Adjnode cn = gv->g->v[i];
        for (; cn != NULL; cn = cn->next) {
            // from curr to location of others
            
            //DrawLine(curr, gv->N[cn->vx], 5, RED);
            // DrawCircleLines(curr.x, gv->N[cn->vx].y, 5, RED);
            

            Prop start = curr;
            Prop end = gv->N[cn->vx];
            //int polarity = 100*(i - cn->vx)*pow((-1), i);
            //Vector2 avg = (Vector2){(start.displacement.x + end.displacement.x)/2.0f, (start.displacement.y + end.displacement.y)/2.0f + polarity};

            DrawLine(start.displacement.x, start.displacement.y, end.displacement.x, end.displacement.y, RED);

            // DrawCircle(start.x, start.y, 10, BLUE);
            // DrawCircle(avg.x, avg.y, 10, BLUE);
            // DrawCircle(end.x, end.y, 10, BLUE);

            

            // Vector2 v[] = {curr.displacement, avg, end.displacement};
            // DrawSplineBezierQuadratic(v, 3, 3, RED);
            //DrawTriangle((Vector2){end.x - 40, end.y - 40}, end, (Vector2){end.x + 40, end.y + 40}, RED);
            
        }

    }
    return 1;
}


int main() {

    int sw = 1080;
    int sh = 720;
    InitWindow(sw, sh, "Graphyne Version:0.01");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(1200);
    
    GraphV gv = test_graph();
    
    //int arr[] = {784/8,128/8,64/8,10};
    int arr[] = {100,50,30,10};
    
    int len_arr = sizeof(arr)/sizeof(arr[0]);
    Vector2 **arrv = neuralNetworkRepEngine(arr, len_arr, (Rectangle){50, 50, 1080, 720}, (Vector2){10, 10});
    // rest len:

    AOEV av = adlToAoe(gv);

    float C = 1000000;
    float k = 0.8;
    int once = 1200;
    int executed = 0;
    float radius = 30;

    //gv->N[0].velocity = (Vector2){12.0f, 12.0f};
    while (!WindowShouldClose()) {  
        float dt = GetFrameTime();

        Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //drawNNrepEngine(arrv, arr, len_arr);
        
        // force on ball i due to j
        // float lenPID_before = 0;
        for (int i = 0; i < gv->g->vertices; i++) {
            if (CheckCollisionPointCircle(mouse, gv->N[i].displacement, radius) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                gv->N[i].displacement.x = mouse.x;
                gv->N[i].displacement.y = mouse.y;
            }
        }

        for (int i = 0; i < gv->g->vertices; i++) {
            float netXF = 0;
            float netYF = 0;
            for (int j = 0; j < gv->g->vertices; j++) {
                if (i != j) { 
                    // F = 1/r^2, O <----> O repulsion, O >----< O attraction 
                    /*
                        O
                          .
                           .
                            .
                             O
                    */
                    float kx = gv->N[i].displacement.x - gv->N[j].displacement.x;
                    float ky = gv->N[i].displacement.y - gv->N[j].displacement.y;
                    float dist = sqrtf(kx*kx + ky*ky);
                
                    float distsq = kx*kx + ky*ky;
                    float xu = kx/dist;
                    float yu = ky/dist;
                   
                    // lenPID_before += lenf;
                    //float lenf = av->edges[edg].len;
                    //printf("%f ", lenf);
                  
                    
                    float lenf = av->edges[i][j].len;

                    // if (lenf < 5) {
                    //     netXF = 0;
                    //     netYF = 0;
                    //     break;
                    // }
                    
                    float force2 = C/(distsq);
                    float force1 = (lenf == -1) ? 0 : -5*k * (dist - lenf/2);


                    netXF += xu * (force1 + force2); 
                    netYF += yu * (force1 + force2); 
                }
            }

            gv->N[i].accleration.x = netXF;
            gv->N[i].accleration.y = netYF;
        }

        // float lenPID_after = 0;
        // for (int i = 0; i < gv->g->vertices; i++)
        // {
        //     for (int j = 0; j < gv->g->vertices; j++)
        //     {
        //          lenPID_after += av->edges[i][j].len;
        //     }
            
        // }
        
        // if (lenPID_after > lenPID_before) {
        //     C *= 10.1;
        //     k *= 2.01;
        // }
        
        // spring: F = kx
        // int edg = 0;
        // for (int i = 0; i < gv->g->vertices; i++) {
        //     float netXF = 0;
        //     float netYF = 0;
        //     Adjnode cn = gv->g->v[i];
        //     for (; cn != NULL; cn = cn->next) {
        //             int j = cn->vx;
        //             // Prop end = gv->N[j];
        //             float kx = gv->N[i].displacement.x - gv->N[j].displacement.x;
        //             float ky = gv->N[i].displacement.y - gv->N[j].displacement.y;
        //             float distsq = kx*kx + ky*ky;
        //             float dist = sqrtf(kx*kx + ky*ky);

        //             float xu = kx/dist;
        //             float yu = ky/dist;
                   
        //             int lenf = 100;
        //             //float lenf = av->edges[edg].len;
        //             //printf("%f ", lenf);
        //             float force2 = 0;
        //             if (dist > 10e-6) {
        //               force2 = 1/(distsq);
        //             }
                    
        //             float force1 = 0.01 * (dist - lenf);


        //             netXF += xu * (force1 + force2); 
        //             netYF += yu * (force1 + force2); 
        //             //edg++;               
        //     }

        //     gv->N[i].accleration.x = netXF;
        //     gv->N[i].accleration.y = netYF;
        // }

        // drag V = -kV
        for (int i = 0; i < gv->g->vertices; i++) {
            gv->N[i].velocity.x += -(5.9 * gv->N[i].velocity.x) * dt;
            gv->N[i].velocity.y += -(5.9 * gv->N[i].velocity.y) * dt;            
        }

        

        for (int i = 0; i < gv->g->vertices; i++) {
            gv->N[i].velocity.x += gv->N[i].accleration.x * dt;
            gv->N[i].velocity.y += gv->N[i].accleration.y * dt;

            gv->N[i].displacement.x += (gv->N[i].velocity.x) * dt;
            gv->N[i].displacement.y += (gv->N[i].velocity.y) * dt;
        }


        graphRendered(gv, (Vector2){200,200},(Vector2){30 + 50, 30 + 50}, radius);

        EndDrawing();
    }

    CloseWindow();

    freeNN(arrv, len_arr);
    freeGraph(gv->g);

    return 0;
}