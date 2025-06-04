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
    // Path *path = find_path(graph);

	// Affichage du chemin
    int path_count = 0;
    Path **paths = findAllPaths(graph, &path_count);

    if (!paths || path_count == 0)
    {
        ft_putstr_fd("Aucun chemin trouvé.\n", 2);
        ft_arna_free();
        return EXIT_FAILURE;
    }

    assignAnts(paths, path_count, graph->nb_fourmis);

    for (int i = 0; i < path_count; i++)
    {
        ft_printf("Chemin %d (%d salles, %d fourmis) : ", i + 1, paths[i]->len, paths[i]->assigned_ants);
        for (int j = 0; j < paths[i]->len; j++)
        {
            Node *node = getNodeByIndex(graph, paths[i]->nodes[j]);
            ft_putstr_fd(node->Nan, 1);
            if (j < paths[i]->len - 1)
                ft_putstr_fd(" -> ", 1);
        }
        ft_putchar_fd('\n', 1);
    }

    multiplePaths(graph, paths, path_count);

    ft_arna_free();
    return EXIT_SUCCESS;
}
