#include "include/lem-in.h"
#include <stdio.h>
#include <time.h>

void print_path(Path *path, Graph *graph) {
    if (!path || !graph) {
        printf("Aucun chemin\n");
        return;
    }
    
    Node *start = getStartNode(graph);
    if (start) {
        printf("%s", start->Nan);
    }
    
    for (int i = 0; i < path->len; i++) {
        Node *node = getNodeByIndex(graph, path->nodes[i]);
        if (node) {
            printf(" -> %s", node->Nan);
        }
    }
    printf(" (longueur: %d)\n", path->len + 1);
}

void demo_bfs_vs_find_path() {
    printf("🔍 DÉMONSTRATION BFS vs FIND_PATH\n");
    printf("==================================\n\n");
    
    // Créer un graphe de test
    Graph *graph = createGraph(10);
    graph->nb_fourmis = 5;
    
    // Ajouter des nœuds
    addNode(graph, "start", 0, 0, true, false);
    addNode(graph, "room1", 1, 0, false, false);
    addNode(graph, "room2", 2, 0, false, false);
    addNode(graph, "room3", 1, 1, false, false);
    addNode(graph, "room4", 2, 1, false, false);
    addNode(graph, "end", 3, 0, false, true);
    
    // Ajouter des connexions pour créer plusieurs chemins
    addEdgeByName(graph, "start", "room1");
    addEdgeByName(graph, "start", "room3");
    addEdgeByName(graph, "room1", "room2");
    addEdgeByName(graph, "room3", "room4");
    addEdgeByName(graph, "room2", "end");
    addEdgeByName(graph, "room4", "end");
    addEdgeByName(graph, "room1", "room3"); // Connexion additionnelle
    
    printf("Graphe créé avec 6 nœuds et plusieurs chemins possibles\n\n");
    
    // Test BFS simple pour le chemin le plus court
    printf("1️⃣ BFS - Chemin le plus court:\n");
    clock_t start_time = clock();
    Path *shortest_path = findShortestPathBFS(graph);
    clock_t end_time = clock();
    double bfs_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    if (shortest_path) {
        printf("   ");
        print_path(shortest_path, graph);
        printf("   Temps d'exécution: %.3f ms\n", bfs_time);
    } else {
        printf("   Aucun chemin trouvé\n");
    }
    
    printf("\n");
    
    // Test find_path standard (qui utilise BFS)
    printf("2️⃣ FIND_PATH - Chemin standard:\n");
    bool used_nodes[graph->node_count];
    for (int i = 0; i < graph->node_count; i++) {
        used_nodes[i] = false;
    }
    
    start_time = clock();
    resetNodePositions(graph);
    Path *standard_path = find_path(graph, used_nodes);
    end_time = clock();
    double find_path_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    if (standard_path) {
        printf("   ");
        print_path(standard_path, graph);
        printf("   Temps d'exécution: %.3f ms\n", find_path_time);
    } else {
        printf("   Aucun chemin trouvé\n");
    }
    
    printf("\n");
    
    // Test findAllPaths (qui utilise maintenant BFS)
    printf("3️⃣ FIND_ALL_PATHS - Tous les chemins (BFS):\n");
    int path_count = 0;
    start_time = clock();
    Path **all_paths = findAllPaths(graph, &path_count);
    end_time = clock();
    double all_paths_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;
    
    printf("   Nombre de chemins trouvés: %d\n", path_count);
    for (int i = 0; i < path_count; i++) {
        printf("   Chemin %d: ", i + 1);
        print_path(all_paths[i], graph);
    }
    printf("   Temps d'exécution: %.3f ms\n", all_paths_time);
    
    printf("\n🎯 COMPARAISON:\n");
    printf("   • BFS simple: %.3f ms\n", bfs_time);
    printf("   • Find path: %.3f ms\n", find_path_time);
    printf("   • All paths: %.3f ms\n", all_paths_time);
    
    printf("\n✅ AVANTAGES DU BFS:\n");
    printf("   • Garantit le chemin le plus court\n");
    printf("   • Exploration systématique niveau par niveau\n");
    printf("   • Meilleure performance pour la recherche de chemins courts\n");
    printf("   • Moins de risque de cycles infinis\n");
}

int main() {
    printf("🚀 DÉMONSTRATION DE L'IMPLÉMENTATION BFS\n");
    printf("========================================\n\n");
    
    demo_bfs_vs_find_path();
    
    printf("\n🎉 Démonstration terminée !\n");
    return 0;
}
