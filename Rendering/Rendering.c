#include "Rendering.h"



AOEV adlToAoe(GraphV gv, enum init_type_fg in, Vector2 bounds) {
    int verts = gv->g->vertices;

    if (in == RANDOM) {
        for (int i = 0; i < verts; i++) {
            gv->N[i] = (Prop){.displacement = (Vector2){GetRandomValue(bounds.x, bounds.y),GetRandomValue(bounds.x, bounds.y)}, (Vector2){0,0}, (Vector2){0,0}};
        }
    } else {
        int width = (int)sqrtf(1.0*verts);
        int row_jump = 0;
        int xd = 5000/verts;
        for (int i = 0; i < verts; i++) {
            int md = i % width;
            if (md == 0) {
                row_jump++;
            }
            gv->N[i] = (Prop){.displacement = (Vector2){bounds.x + md * xd , bounds.y + md + xd * row_jump}, (Vector2){0,0}, (Vector2){0,0}};
        }
    }

    AOEV av = malloc(sizeof(struct arrayOfEdgesV));
    

    int edges = 0;
    for (int i = 0; i < verts; i++) {
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
            ed[i][j].len = -1;                                                            
        }
    }

    for (int i = 0; i < verts; i++) {
        Adjnode cn = gv->g->v[i];
        for (; cn != NULL; cn = cn->next) {
            int j = cn->vx; 
            float dx = gv->N[i].displacement.x - gv->N[j].displacement.x;
            float dy = gv->N[i].displacement.y - gv->N[j].displacement.y;      
            ed[i][j].len = sqrtf(dx*dx + dy*dy);
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

GraphV test_graph(int num_nodes) {
    GraphV gv = malloc(sizeof(struct graphV));
    gv->g = newGraph(num_nodes);
    gv->N = malloc(sizeof(Prop) * (gv->g->vertices));
    // representation scheme
    // NN, circular, adaptive(electromagnetic repulsion)
    // int cont = 0;

    

    for (int i = 0; i < gv->g->vertices; i++)
    {
        gv->N[i] = (Prop){.displacement = (Vector2){0,0}, (Vector2){0,0}, (Vector2){0,0}};
    }
    

    // for spacing out: (Vector2){i*150 + 100,GetRandomValue(300, 400)}
    // gv->g = insertEdge(gv->g, 0, 1);
    // gv->g = insertEdge(gv->g, 1, 2);
    // gv->g = insertEdge(gv->g, 2, 3);
    // gv->g = insertEdge(gv->g, 3, 4);
    // gv->g = insertEdge(gv->g, 4, 0);
    // gv->g = insertEdge(gv->g, 4, 2);
    // gv->g = insertEdge(gv->g, 2, 4);
    // gv->g = insertEdge(gv->g, 0, 3);
    // gv->g = insertEdge(gv->g, 1, 4);
    // gv->g = insertEdge(gv->g, 4, 0);
    // gv->g = insertEdge(gv->g, 3, 1);
    // gv->g = insertEdge(gv->g, 5, 2);
    // gv->g = insertEdge(gv->g, 5, 1);


    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++)
        {
            if (i != j) {
                //printf("%d %d\n", i, j);
                gv->g = insertEdge(gv->g, i, j);
            }
        }   
    }
    
    //printGraph(gv->g);
    return gv;
}


int graphRendered(GraphV gv, Vector2 offset, Vector2 start, float radius, int BALL_FONT) {
    for (int i = 0; i < gv->g->vertices; i++) {
        //gv->N[i] = (Vector2) {gv->N[i].x + start.x*i + offset.x , gv->N[i].y + start.y + offset.y};
        Prop curr = gv->N[i];
        DrawCircle(curr.displacement.x, curr.displacement.y, radius, BLACK);
        DrawText(TextFormat("%d", i), curr.displacement.x - BALL_FONT/2, curr.displacement.y - BALL_FONT/2, BALL_FONT, WHITE);
       
        Adjnode cn = gv->g->v[i];
        for (; cn != NULL; cn = cn->next) {
            // from curr to location of others
            // DrawLine(curr, gv->N[cn->vx], 5, RED);
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
    return 0;
}

void graphLine(GraphV gv, Vector2 offset, Vector2 start, int BALL_FONT, int orientation) {
    for (int i = 0; i < gv->g->vertices; i++) {
        if (orientation == 0) {
            gv->N[i].displacement = (Vector2) {start.x*i + offset.x , start.y + offset.y};
        } else {
            gv->N[i].displacement = (Vector2) {start.x + offset.x , start.y*i + offset.y};
        }
    }
}

void graphCircular(GraphV gv, Vector2 offset, int BALL_FONT, float scaler) {
    // goto that node offset the, calcualte x = rcos(theta/n), y = rsin(theta/n)
    int n = gv->g->vertices;
    // r theta -> circumference 
    float val = 2*PI/n;
    float r = scaler / val;
    for (int i = 0; i < gv->g->vertices; i++) {
        float x = r * cosf(val * i) + offset.x;
        float y = r * sinf(val * i) + offset.y;
        gv->N[i].displacement = (Vector2) {x, y};
    }
}


int graphBezierConnection(GraphV gv, Vector2 offset, Vector2 start, float radius, int BALL_FONT, int alternate) {
    for (int i = 0; i < gv->g->vertices; i++) {
        Prop curr = gv->N[i];
        Adjnode cn = gv->g->v[i];
        for (; cn != NULL; cn = cn->next) {
            // from curr to location of others
            //DrawLine(curr.displacement.x, gv->N[cn->vx].displacement.x, 5, RED);
            //DrawCircleLines(curr.x, gv->N[cn->vx].y, 5, RED);
            Prop start = curr;
            Prop end = gv->N[cn->vx];
            int polarity = (alternate == 1) ? 100*(i - cn->vx)*pow((-1), i) : 0;
            Vector2 avg = (Vector2){(start.displacement.x + end.displacement.x)/2.0f, (start.displacement.y + end.displacement.y)/2.0f + polarity};

            //DrawLine(start.displacement.x, start.displacement.y, end.displacement.x, end.displacement.y, RED);
            // DrawCircle(start.displacement.x, start.displacement.y, 10, BLUE);
            // DrawCircle(avg.x, avg.y, 10, BLUE);
            // DrawCircle(end.displacement.x, end.displacement.y, 10, BLUE);
            Vector2 v[] = {curr.displacement, avg, end.displacement};
            DrawSplineBezierQuadratic(v, 3, 3, RED);
            //DrawTriangle((Vector2){end.displacement.x - 40, end.displacement.y - 40}, end.displacement, (Vector2){end.displacement.x + 40, end.displacement.y + 40}, RED);
            
        }

        DrawCircle(curr.displacement.x, curr.displacement.y, radius, BLACK);
        DrawText(TextFormat("%d", i), curr.displacement.x - BALL_FONT/2, curr.displacement.y - BALL_FONT/2, BALL_FONT, WHITE);

    }
    return 1;
}




void dragNode(GraphV gv, Vector2 mouse, float radius) {
    for (int i = 0; i < gv->g->vertices; i++) {
        if (CheckCollisionPointCircle(mouse, gv->N[i].displacement, radius) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            gv->N[i].displacement.x = mouse.x;
            gv->N[i].displacement.y = mouse.y;
        }
    }
}

void updateElectrostaticSpringForce(GraphV gv, AOEV av, float C, float k) {
    for (int i = 0; i < gv->g->vertices; i++) {
        float netXF = 0;
        float netYF = 0;
        for (int j = 0; j < gv->g->vertices; j++) {
            if (i != j) { 
                
                float kx = gv->N[i].displacement.x - gv->N[j].displacement.x;
                float ky = gv->N[i].displacement.y - gv->N[j].displacement.y;
                float dist = sqrtf(kx*kx + ky*ky);
            
                float distsq = kx*kx + ky*ky;
                float xu = kx/dist;
                float yu = ky/dist;
                float lenf = av->edges[i][j].len;
               
                float force1 = (lenf == -1) ? 0 : -k * (dist - lenf/2);
                
                
                float force2 = C/(distsq);

                netXF += xu * (force1 + force2); 
                netYF += yu * (force1 + force2); 
            }
        }

        gv->N[i].accleration.x = netXF;
        gv->N[i].accleration.y = netYF;
    }
}

void applyDrag(GraphV gv, float dt, float drag_constant) {
    for (int i = 0; i < gv->g->vertices; i++) {
        gv->N[i].velocity.x -= (drag_constant * gv->N[i].velocity.x) * dt;
        gv->N[i].velocity.y -= (drag_constant * gv->N[i].velocity.y) * dt;            
    }
}

void updateLoopIntegral(GraphV gv, float dt) {
    for (int i = 0; i < gv->g->vertices; i++) {
        gv->N[i].velocity.x += gv->N[i].accleration.x * dt;
        gv->N[i].velocity.y += gv->N[i].accleration.y * dt;

        gv->N[i].displacement.x += (gv->N[i].velocity.x) * dt;
        gv->N[i].displacement.y += (gv->N[i].velocity.y) * dt;
    }
}