#include "lem-in.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        // return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    (void)filename; // Suppress unused variable warning

    Graph* graph = parsigense(0);
    if (!graph) {
        printf("Error: Failed to parse input\n");
        return EXIT_FAILURE;
    }

    printf("✅ Parsing completed successfully\n");
    
    // Here you would typically process the graph, e.g., find paths, etc.
    // For demonstration, we will just print the size of the graph
    printf("Number of nodes: %d\n", graph->node_count);
    printf("Graph capacity: %d\n", graph->size);
    printf("Number of ants: %d\n", graph->nb_fourmis);
    
    // Afficher le graphe parsé
    displayGraph(graph);
    
    // Free allocated resources if necessary
    // For example, if you had dynamically allocated memory in the graph structure
    ft_arna_free();
    return EXIT_SUCCESS;
}