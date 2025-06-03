#include "data.h"


Graph* createGraph(int size) {
    Graph* graph = (Graph*)ft_arnalloc(sizeof(Graph));
    if (!graph) {
        return NULL;
    }
    
    graph->size = size;         // Capacity (for test compatibility)
    graph->node_count = 0;      // Number of used nodes
    graph->size_max = size;     // Initial max capacity
    graph->nb_fourmis = 0;
    graph->adjacencyList = (Node**)ft_arnalloc(size * sizeof(Node*));
    
    if (!graph->adjacencyList) {
        return NULL;
    }
    
    for (int i = 0; i < size; i++) {
        graph->adjacencyList[i] = NULL;
    }
    return graph;
}