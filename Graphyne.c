#include "Rendering/Rendering.h"

#define BALL_FONT 20




int main() {

    int sw = 1080;
    int sh = 720;
    InitWindow(sw, sh, "Graphyne Version:0.01");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(1200);
    
    GraphV gv = test_graph(20);
    
    int arr[] = {784/8,128/8,64/8,10};
    //int arr[] = {40,20,10,5};
    
    int len_arr = sizeof(arr)/sizeof(arr[0]);
    Vector2 **arrv = neuralNetworkRepEngine(arr, len_arr, (Rectangle){50, 50, 1080, 720}, (Vector2){10, 10});
    // rest len:

    AOEV av = adlToAoe(gv);

    float C = 100000000;
    float k = 0.7;

    float radius = 20;

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
                    float lenf = av->edges[i][j].len;
                    float force1 = 0;

                    if (lenf != -1) {
                        force1 = (lenf == -1) ? 0 : -5*k * (dist - lenf/2);
                    }
                    
                    float force2 = C/(distsq);

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


        graphRendered(gv, (Vector2){200,200},(Vector2){30 + 50, 30 + 50}, radius, BALL_FONT);
        EndDrawing();
    }

    CloseWindow();

    freeNN(arrv, len_arr);
    freeGraph(gv->g);

    return 0;
}