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
        
        // Afficher l'erreur spécifique si elle existe
        if (has_parsing_error()) {
            print_parsing_error();
        }
        
        return EXIT_FAILURE;
    }

    // printf("✅ Parsing completed successfully\n");
    
    // // Here you would typically process the graph, e.g., find paths, etc.
    // // For demonstration, we will just print the size of the graph
    // printf("Number of nodes: %d\n", graph->node_count);
    // printf("Graph capacity: %d\n", graph->size);
    // printf("Number of ants: %d\n", graph->nb_fourmis);
    
    // // Afficher le graphe parsé
    // displayGraph(graph);
    Path *path = find_path(graph);

	// Affichage du chemin
	  if (path)
	  {
		  ft_putstr_fd("Chemin trouvé : ", 1);
		  for (int i = 0; i < path->len; i++)
		 {
			  Node *node = getNodeByIndex(graph, path->nodes[i]);
			  ft_putstr_fd(node->Nan, 1);
			  if (i < path->len - 1)
				  ft_putstr_fd(" -> ", 1);
		  }
		  ft_putchar_fd('\n', 1);
	  }
	  else
		  ft_putstr_fd("Aucun chemin trouvé.\n", 2);
    
    // Free allocated resources if necessary
    // For example, if you had dynamically allocated memory in the graph structure
    ft_arna_free();
    return EXIT_SUCCESS;
}
