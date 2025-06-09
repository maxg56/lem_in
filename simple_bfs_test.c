#include "include/lem-in.h"
#include <stdio.h>

void simple_bfs_test() {
    printf("🔍 TEST SIMPLE BFS\n");
    printf("==================\n\n");
    
    // Créer un graphe simple
    Graph *graph = createGraph(5);
    if (!graph) {
        printf("Erreur: Impossible de créer le graphe\n");
        return;
    }
    
    graph->nb_fourmis = 3;
    
    // Ajouter des nœuds
    addNode(graph, "start", 0, 0, true, false);
    addNode(graph, "middle", 1, 0, false, false);
    addNode(graph, "end", 2, 0, false, true);
    
    // Ajouter des connexions
    addEdgeByName(graph, "start", "middle");
    addEdgeByName(graph, "middle", "end");
    
    printf("Graphe créé: start -> middle -> end\n");
    printf("Nombre de fourmis: %d\n\n", graph->nb_fourmis);
    
    // Test du BFS le plus court
    printf("Test BFS pour le chemin le plus court:\n");
    Path *path = findShortestPathBFS(graph);
    
    if (path) {
        printf("✅ Chemin trouvé avec longueur: %d\n", path->len + 1);
        printf("   Nœuds du chemin: start");
        for (int i = 0; i < path->len; i++) {
            Node *node = getNodeByIndex(graph, path->nodes[i]);
            if (node) {
                printf(" -> %s", node->Nan);
            }
        }
        printf("\n");
    } else {
        printf("❌ Aucun chemin trouvé\n");
    }
    
    printf("\n✅ Test BFS terminé avec succès!\n");
}

int main() {
    printf("🚀 DÉMONSTRATION BFS SIMPLE\n");
    printf("===========================\n\n");
    
    simple_bfs_test();
    
    return 0;
}
