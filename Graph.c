#include "Graph.h"



 

// initialise graph
Graph newGraph(int vertices) {
    struct graph* g = malloc(sizeof(struct graph));
    g->v = NULL;

    Adjnode *v = malloc(sizeof(Adjnode) * vertices);
    g->v = v;

    for (int i = 0; i < vertices; i++) {
        g->v[i] = NULL;
    }

    g->vertices = vertices;

    //printf("%x %d", v, sizeof(v));

    return g;
}

static Adjnode newNode(Vertex vx, int value) {
    Adjnode v = malloc(sizeof(struct node));
    v->value = value;
    v->vx = vx;
    v->next = NULL;
    return v;
}

static void freeNode(struct node *n) {
    if (n != NULL) free(n);
    return; 
}

// free graph
void freeGraph(Graph g) {
    for (int i = 0; i < g->vertices; i++) {
        Adjnode curr = g->v[i];
        // Adjnode mover = NULL;
        while (curr != NULL) {
            struct node *hold = curr->next;
            freeNode(curr);
            curr = hold;
        }
    }

    free(g->v);
    free(g);
}


// assume that passed vertices are valid
bool containsEdge(Graph g, Vertex v, Vertex w) {
    Adjnode curr = g->v[v];

    while (curr != NULL) {

        if (curr->vx == w) {
            return true;
        }

        curr = curr->next;
    }

    // for (Adjnode curr = g->v[v]; curr != NULL; curr = curr->next) {
    //     if (curr->vx == w) {
    //         return true;
    //     }
    // }
    return false;
}




// insert edge beteen 2 valid verticess v -> w, insert w in v, inserts at head
Graph insertEdge(Graph g, Vertex v, Vertex w) {

    if (containsEdge(g, v, w) || 
        (!validVertex(g, v) || !validVertex(g, w))) 
        return g;


    struct node *head = g->v[v];
    struct node *new = newNode(w, 0);

    
    g->v[v] = new;
    
    if (head != NULL) {
        new->next = head->next;
    } 


    return g;
}


// Check if vertex is valid, 0 to g->vertices - 1
bool validVertex(Graph g, Vertex v) {
    if (v < 0  || v >= g->vertices) {
        return false;
    }
    return true;
}



void printGraph(Graph g) {
    printf("Graph has %d vertices(x)\n", g->vertices);

    for (int i = 0; i < g->vertices; i++) {
        Adjnode curr = g->v[i];
        printf("%d-->", i);
        while (curr != NULL) {
            printf("%d ", curr->vx);
            curr = curr->next;
        }
        // for (;curr != NULL; curr = curr->next) {
        //     printf("%d ", curr->vx);
        // }
        printf("\n");
    }

    return;
}



