#include "lem-in.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Graph* graph = NULL;
    
    // Clear any previous parsing errors
    clear_parsing_error();
    
    if (argc == 2) {
        const char *filename = argv[1];
        graph = parse_from_file(filename);
    }
    else {
        graph = parsigense(0); 
    }

    if (!graph) {
        printf("Error: Failed to parse input\n");
        
        // Afficher l'erreur spécifique si elle existe
        if (has_parsing_error()) {
            print_parsing_error();
        }
        
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