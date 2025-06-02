#include "data.h"

void displayGraph(Graph* graph) {
    if (!graph || !graph->adjacencyList) {
        return; // Invalid graph
    }

    for (int i = 0; i < graph->size; i++) {
        Node* currentNode = graph->adjacencyList[i];
        if (currentNode) {
            printf("Node %d: %s (x: %d, y: %d, Start: %s, End: %s)\n", 
                   i, currentNode->Nan, currentNode->x, currentNode->y,
                   currentNode->isStart ? "Yes" : "No",
                   currentNode->isEnd ? "Yes" : "No");
        } else {
            printf("Node %d: NULL\n", i);
        }
    }
}