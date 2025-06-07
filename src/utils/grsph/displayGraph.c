#include "data.h"

void displayGraph(Graph* graph) {
    if (!graph || !graph->adjacencyList) {
        return; // Invalid graph
    }

    printf("\n=== GRAPH STRUCTURE ===\n");
    for (int i = 0; i < graph->node_count; i++) {
        Node* currentNode = graph->adjacencyList[i];
        if (currentNode) {
            printf("Node %d: %s (x: %d, y: %d, Start: %s, End: %s)\n", 
                i, currentNode->Nan, currentNode->x, currentNode->y,
                currentNode->isStart ? "Yes" : "No",
                currentNode->isEnd ? "Yes" : "No");
            
            // Display connections
            Node* connection = currentNode->next;
            if (connection) {
                printf("  Connections:");
                while (connection) {
                    printf(" -> %s", connection->Nan);
                    connection = connection->next;
                }
                printf("\n");
            } else {
                printf("  No connections\n");
            }
        } else {
            printf("Node %d: NULL\n", i);
        }
    }
    printf("=======================\n\n");
}