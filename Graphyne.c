#include "Rendering/Rendering.h"

#define BALL_FONT 20




int main() {

    int sw = 1080;
    int sh = 720;
    InitWindow(sw, sh, "Graphyne Version:0.01");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    //SetTargetFPS(0);
    
    GraphV gv = test_graph(10);
    GraphV gc = test_graph(20);
    
    int arr[] = {78,12,6,1};
    
    int len_arr = sizeof(arr)/sizeof(arr[0]);
    Vector2 **arrv = neuralNetworkRepEngine(arr, len_arr, (Rectangle){50, 50, 1080, 720}, (Vector2){10, 10});
  

    
    RLM ac = forceGraphInit(gc, RANDOM, (Vector2){100, 500});

    float C = 100000000;
    float k = 5;
    float radius = 20;
    float drag_constant = 6.0;

    //gv->N[0].velocity = (Vector2){12.0f, 12.0f};
    while (!WindowShouldClose()) {  
        float dt = GetFrameTime();
        Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        graphCircular(gv, (Vector2){1500,200}, BALL_FONT, 50);
        drawNNrepEngine(arrv, arr, len_arr);
        
        
        // graphCircular(gc, (Vector2){1500,100}, BALL_FONT, 50);
        dragNode(gc, mouse, radius);
        updateElectrostaticSpringForce(gc,ac,C,k);
        applyDrag(gc, dt, drag_constant);
        updateLoopIntegral(gc, dt);
        
        graphRendered(gv, (Vector2){200,200},(Vector2){30 + 50, 30 + 50}, radius, BALL_FONT);
        graphRendered(gc, (Vector2){500,500},(Vector2){30 + 50, 30 + 50}, radius, BALL_FONT);
        //graphRendered(gc, (Vector2){200,200},(Vector2){30 + 50, 30 + 50}, radius, BALL_FONT);
        //graphBezierConnection(gv, (Vector2){200,200},(Vector2){30 + 50, 30 + 50}, radius, BALL_FONT, 1);
        EndDrawing();
    }

    CloseWindow();
    freeNN(arrv, len_arr);
    freeGraph(gv->g);
    freeGraph(gc->g);
    freeRLM(ac);

    return 0;
}