#include "include/data.h"
#include <stdio.h>

void resetNodePosition(void);

int main(void) {
    printf("=== SIMULATING EXACT TEST PATTERN ===\n");
    
    // Simulate the exact pattern from the failing test
    resetNodePosition(); // Reset position counter
    Graph* graph = createGraph(2);
    
    if (graph) {
        printf("Graph created, adding nodes...\n");
        addNode(graph, "start", 0, 0, true, false);
        addNode(graph, "end", 10, 10, false, true);
        
        printf("Nodes added, calling displayGraph...\n");
        printf("    Testing displayGraph output (should show 2 nodes):\n");
        printf("    "); // Indent for visual clarity
        displayGraph(graph);
        
        printf("displayGraph completed successfully!\n");
    }
    
    ft_arna_free();
    printf("Test completed!\n");
    return 0;
}
