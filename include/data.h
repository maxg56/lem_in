#ifndef DATA_H
# define DATA_H

#include <stdbool.h>
#include <stdlib.h>
#  include "libft.h"


typedef struct Node {
    char *Nan;
    bool isStart;
    bool isEnd;
    int x;
    int y;
    struct Node* next;
} Node;

typedef struct Graph {
    int size;           // Capacity (for compatibility with tests)
    int node_count;     // Number of actually used nodes
    int size_max;       // Maximum capacity for dynamic resizing
    int nb_fourmis;
    Node** adjacencyList;
} Graph;

Graph* createGraph(int size);
void displayGraph(Graph* graph);
void addNode(Graph* graph, char * Nan, int x , int y, bool isStart, bool isEnd);
void resetNodePosition(void);

// Gestion des connexions/arêtes
void addEdge(Graph* graph, int nodeA, int nodeB);
void addEdgeByName(Graph* graph, char* nameA, char* nameB);
bool hasEdge(Graph* graph, int nodeA, int nodeB);
void removeEdge(Graph* graph, int nodeA, int nodeB);

// Recherche et navigation
int findNodeByName(Graph* graph, char* name);
Node* getNodeByIndex(Graph* graph, int index);
Node* getStartNode(Graph* graph);
Node* getEndNode(Graph* graph);
int getNodeCount(Graph* graph);

// Fonctions d'adjacence
Node** getNeighbors(Graph* graph, int nodeIndex, int* count);
int getNeighborCount(Graph* graph, int nodeIndex);
bool areNeighbors(Graph* graph, int nodeA, int nodeB);

// Validation
bool validateGraph(Graph* graph);
bool hasStartEnd(Graph* graph);
bool isGraphConnected(Graph* graph);
bool isConnected(Graph* graph, int nodeA, int nodeB);

// Resize functions
bool resizeGraph(Graph* graph, int newSize);

#endif
