#include "data.h"


Graph* createGraph(int size) {
    Graph* graph = (Graph*)ft_arnalloc(sizeof(Graph));
    if (!graph) {
        return NULL; // Memory allocation failed
    }
    
    graph->size = size;
    graph->nb_fourmis = 0; // Initialiser le nombre de fourmis
    graph->adjacencyList = (Node**)ft_arnalloc(size * sizeof(Node*));
    
    if (!graph->adjacencyList) {
        return NULL; // Memory allocation failed
    }
    
    for (int i = 0; i < size; i++) {
        graph->adjacencyList[i] = NULL; // Initialize each list to NULL
    }
    
    return graph;
}