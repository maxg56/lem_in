#include "lem-in.h"
#include <stdio.h>
#include <time.h>

static void display_paths_if_needed(Graph *graph, Path **paths, int path_count);
static void execute_ant_simulation(Graph *graph, Path **paths, int path_count);
static void display_final_statistics(Graph *graph, int path_count, clock_t start_time);

int main(int argc, char *argv[]) {
    Graph* graph = NULL;
    char *filename = NULL;
    clock_t start_time = 0;
    
    init_flags();
    
    if (!parse_flags(argc, argv, &filename)) {
        return EXIT_FAILURE;
    }
    
    if (is_help()) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }
    
    // Démarrer le benchmark si demandé
    if (is_benchmark()) {
        start_time = clock();
        verbose_printf("Démarrage du benchmark...\n");
    }
    
    clear_parsing_error();
    
    verbose_printf("Début du parsing...\n");
    
    
    if (filename) {
        debug_printf("Lecture du fichier: %s\n", filename);
        graph = parse_from_file(filename);
    }
    else {
        debug_printf("Lecture depuis stdin\n");
        graph = parsigense(0); 
    }

    if (!graph) {
        
        if (!is_quiet()) {
            // Afficher l'erreur spécifique si elle existe
            if (has_parsing_error()) {
                print_parsing_error();
            }
        }
        
        return EXIT_FAILURE;
    }

    verbose_printf("Parsing completed successfully\n");
    
    if (is_debug()) {
        debug_printf("Number of nodes: ");
        ft_putnbr_fd(graph->node_count, 1);
        ft_putchar_fd('\n', 1);
        debug_printf("Graph capacity: ");
        ft_putnbr_fd(graph->size, 1);
        ft_putchar_fd('\n', 1);
        debug_printf("Number of ants: ");
        ft_putnbr_fd(graph->nb_fourmis, 1);
        ft_putchar_fd('\n', 1);
    }
    
    // Affichage du graphe si en mode debug
    if (is_debug()) {
        debug_printf("Affichage du graphe:\n");
        displayGraph(graph);
    }

    // Trouver tous les chemins disponibles
    verbose_printf("Recherche des chemins...\n");
    int path_count = 0;
    Path **paths = findAllPaths(graph, &path_count);

    if (!paths || path_count == 0) {
        if (!is_quiet()) {
            ft_putstr_fd("Aucun chemin trouvé.\n", 2);
        }
        ft_arna_free();
        return EXIT_FAILURE;
    }

    verbose_printf("Chemins trouvés: ");
    if (is_verbose() && !is_quiet()) {
        ft_putnbr_fd(path_count, 1);
        ft_putchar_fd('\n', 1);
    }

    verbose_printf("Attribution des fourmis aux chemins...\n");
    assignAnts(paths, path_count, graph->nb_fourmis);
    display_paths_if_needed(graph, paths, path_count);
    
    // Skip simulation if only showing used paths
    if (!show_used_paths()) {
        execute_ant_simulation(graph, paths, path_count);
    }
    
    display_final_statistics(graph, path_count, start_time);
    ft_arna_free();
    return EXIT_SUCCESS;
}

// Fonctions auxiliaires pour nettoyer le main
static void display_paths_if_needed(Graph *graph, Path **paths, int path_count)
{
    // Afficher tous les chemins trouvés avec --all-paths
    if (show_all_paths() && !is_quiet()) {
        ft_putstr_fd("\n=== TOUS LES CHEMINS TROUVÉS ===\n", 1);
        for (int i = 0; i < path_count; i++) {
            ft_putstr_fd("Chemin ", 1);
            ft_putnbr_fd(i + 1, 1);
            ft_putstr_fd(" (", 1);
            ft_putnbr_fd(paths[i]->len, 1);
            ft_putstr_fd(" salles) : ", 1);
            for (int j = 0; j < paths[i]->len; j++) {
                Node *node = getNodeByIndex(graph, paths[i]->nodes[j]);
                ft_putstr_fd(node->Nan, 1);
                if (j < paths[i]->len - 1) {
                    ft_putstr_fd(" -> ", 1);
                }
            }
            ft_putchar_fd('\n', 1);
        }
    }
    
    // Afficher seulement les chemins utilisés avec --used-paths
    if (show_used_paths() && !is_quiet()) {
        ft_putstr_fd("\n=== CHEMINS UTILISÉS ===\n", 1);
        for (int i = 0; i < path_count; i++) {
            if (paths[i]->assigned_ants > 0) {
                ft_putstr_fd("Chemin ", 1);
                ft_putnbr_fd(i + 1, 1);
                ft_putstr_fd(" (", 1);
                ft_putnbr_fd(paths[i]->len, 1);
                ft_putstr_fd(" salles, ", 1);
                ft_putnbr_fd(paths[i]->assigned_ants, 1);
                ft_putstr_fd(" fourmis) : ", 1);
                for (int j = 0; j < paths[i]->len; j++) {
                    Node *node = getNodeByIndex(graph, paths[i]->nodes[j]);
                    ft_putstr_fd(node->Nan, 1);
                    if (j < paths[i]->len - 1) {
                        ft_putstr_fd(" -> ", 1);
                    }
                }
                ft_putchar_fd('\n', 1);
            }
        }
    }
    
    // Comportement existant pour -p et -d
    if ((show_paths() || is_debug()) && !show_all_paths() && !show_used_paths()) {
        if (!is_quiet()) {
            for (int i = 0; i < path_count; i++) {
                if (is_debug()) {
                    debug_printf("Chemin ");
                    ft_putnbr_fd(i + 1, 1);
                    ft_putstr_fd(" (", 1);
                    ft_putnbr_fd(paths[i]->len, 1);
                    ft_putstr_fd(" salles, ", 1);
                    ft_putnbr_fd(paths[i]->assigned_ants, 1);
                    ft_putstr_fd(" fourmis) : ", 1);
                }
                for (int j = 0; j < paths[i]->len; j++) {
                    Node *node = getNodeByIndex(graph, paths[i]->nodes[j]);
                    ft_putstr_fd(node->Nan, 1);
                    if (j < paths[i]->len - 1 && (show_paths() || is_debug())) {
                        ft_putstr_fd(" -> ", 1);
                    }
                }
                ft_putchar_fd('\n', 1);
            }
        }
    }
}

static void execute_ant_simulation(Graph *graph, Path **paths, int path_count)
{
    verbose_printf("Simulation du mouvement des fourmis...\n");
    multiplePaths(graph, paths, path_count);
}

static void display_final_statistics(Graph *graph, int path_count, clock_t start_time)
{
    // Afficher les statistiques si demandées
    if (show_stats() && !is_quiet()) {
        ft_putstr_fd("\n=== STATISTIQUES ===\n", 1);
        ft_putstr_fd("Nombre de fourmis: ", 1);
        ft_putnbr_fd(graph->nb_fourmis, 1);
        ft_putchar_fd('\n', 1);
        ft_putstr_fd("Nombre de chemins: ", 1);
        ft_putnbr_fd(path_count, 1);
        ft_putchar_fd('\n', 1);
        ft_putstr_fd("Nombre de salles: ", 1);
        ft_putnbr_fd(graph->node_count, 1);
        ft_putchar_fd('\n', 1);
        ft_putstr_fd("Algorithme de recherche: BFS (Breadth-First Search)\n", 1);
    }
    
    // Afficher le temps d'exécution si benchmark
    if (is_benchmark()) {
        clock_t end_time = clock();
        double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        
        if (!is_quiet()) {
            ft_putstr_fd("\n=== BENCHMARK ===\n", 1);
            ft_putstr_fd("Temps total: ", 1);
            ft_putstr_fd("~", 1);
            ft_putnbr_fd((int)(total_time * 1000), 1);
            ft_putstr_fd("ms\n", 1);
        }
    }
}
