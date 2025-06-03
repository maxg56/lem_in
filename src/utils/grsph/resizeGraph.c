#include "data.h"

bool resizeGraph(Graph* graph, int newSize) {
    if (newSize <= graph->size_max) {
        return true; // Already big enough
    }
    
    Node** newAdjacencyList = (Node**)ft_arnalloc(newSize * sizeof(Node*));
    if (!newAdjacencyList) {
        return false; // Memory allocation failed
    }
    
    // Copy existing nodes
    for (int i = 0; i < graph->size_max; i++) {
        newAdjacencyList[i] = graph->adjacencyList[i];
    }
    
    // Initialize new slots to NULL
    for (int i = graph->size_max; i < newSize; i++) {
        newAdjacencyList[i] = NULL;
    }
    
    graph->adjacencyList = newAdjacencyList;
    graph->size_max = newSize;
    return true;
}
