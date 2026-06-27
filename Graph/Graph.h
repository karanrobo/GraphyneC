#ifndef GVIS_H
#define GVIS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>


typedef struct graph *Graph;
typedef struct node *Adjnode; 
typedef int Vertex;


// graph
struct graph {
    struct node **v; // pointer to array of pointers 
    int vertices;

};

// graph rep adjacency list 
struct node {
    struct node *next;
    Vertex vx;
    int value;
};

// initialise graph
Graph newGraph(int vertices);

// free graph
void freeGraph(Graph g);

// check if LL v contains w
bool containsEdge(Graph g, Vertex v, Vertex w);


// insert edge beteen 2 valid verticess
Graph insertEdge(Graph g, Vertex v, Vertex w);


// Check if vertex is valid
bool validVertex(Graph g, Vertex v);

// print graph;
void printGraph(Graph g);


#endif