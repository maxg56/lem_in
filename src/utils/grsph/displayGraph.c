#include "data.h"

void displayGraph(Graph* graph) {
    if (!graph || !graph->adjacencyList) {
        return; // Invalid graph
    }

    printf("\n=== GRAPH STRUCTURE ===\n");
    for (int i = 0; i < graph->node_count; i++) {
        Node* currentNode = graph->adjacencyList[i];
        if (currentNode) {
            // Add comprehensive safety checks
            if (currentNode->Nan == NULL) {
                printf("Node %d: [NULL_NAME] (x: %d, y: %d, Start: %s, End: %s)\n", 
                    i, currentNode->x, currentNode->y,
                    currentNode->isStart ? "Yes" : "No",
                    currentNode->isEnd ? "Yes" : "No");
            } else {
                printf("Node %d: %s (x: %d, y: %d, Start: %s, End: %s)\n", 
                    i, currentNode->Nan, currentNode->x, currentNode->y,
                    currentNode->isStart ? "Yes" : "No",
                    currentNode->isEnd ? "Yes" : "No");
            }
            
            // Display connections
            Node* connection = currentNode->next;
            if (connection) {
                printf("  Connections:");
                while (connection) {
                    const char* connectionName = (connection->Nan != NULL) ? connection->Nan : "[NULL_NAME]";
                    printf(" -> %s", connectionName);
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