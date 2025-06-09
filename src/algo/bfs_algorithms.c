/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_algorithms.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"
#include <limits.h>

// Structure pour stocker les chemins partiels pendant le BFS
typedef struct PathNode {
    int *path;
    int length;
    struct PathNode *next;
} PathNode;

// Fonction BFS pour trouver un chemin en utilisant une queue simple
Path *find_path(Graph *graph, bool *used_nodes) {
    if (!graph) return NULL;

    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);
    if (!startNode || !endNode) return NULL;

    int start = findNodeByName(graph, startNode->Nan);
    int end = findNodeByName(graph, endNode->Nan);
    if (start == -1 || end == -1) return NULL;

    int queue[graph->node_count];
    int front = 0, rear = 0;
    queue[rear++] = start;
    startNode->visited = true;
    startNode->parentNode = -1;

    while (front < rear) {
        int current_index = queue[front++];
        if (current_index == end) break;

        int count = 0;
        Node **neighbors = getNeighbors(graph, current_index, &count);
        for (int i = 0; i < count; i++) {
            int ni = findNodeByName(graph, neighbors[i]->Nan);
            if (ni != -1 && !neighbors[i]->visited) {
                bool can_use = true;
                if (!neighbors[i]->isStart && !neighbors[i]->isEnd && used_nodes[ni]) {
                    can_use = false;
                }

                if (can_use) {
                    neighbors[i]->visited = true;
                    neighbors[i]->parentNode = current_index;
                    queue[rear++] = ni;
                }
            }
        }
    }

    // Check if we actually reached the end node
    if (!endNode->visited) {
        return NULL; // No path found
    }

    return build_path(graph, end);
}

// BFS simple pour trouver le chemin le plus court
Path *findShortestPathBFS(Graph *graph)
{
    if (!graph) return NULL;

    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);
    if (!startNode || !endNode) return NULL;

    int start = findNodeByName(graph, startNode->Nan);
    int end = findNodeByName(graph, endNode->Nan);
    if (start == -1 || end == -1) return NULL;

    // Réinitialiser les nœuds
    resetNodePositions(graph);

    // Queue pour BFS
    int queue[graph->node_count];
    int front = 0, rear = 0;
    
    // Commencer par le nœud de départ
    queue[rear++] = start;
    startNode->visited = true;
    startNode->parentNode = -1;

    bool found = false;
    
    // BFS standard
    while (front < rear && !found) {
        int current_index = queue[front++];
        
        if (current_index == end) {
            found = true;
            break;
        }

        int count = 0;
        Node **neighbors = getNeighbors(graph, current_index, &count);
        
        for (int i = 0; i < count; i++) {
            int neighbor_index = findNodeByName(graph, neighbors[i]->Nan);
            if (neighbor_index != -1 && !neighbors[i]->visited) {
                neighbors[i]->visited = true;
                neighbors[i]->parentNode = current_index;
                queue[rear++] = neighbor_index;
            }
        }
    }

    if (!found) {
        return NULL; // Aucun chemin trouvé
    }

    return build_path(graph, end);
}

// BFS simple pour trouver plusieurs chemins sans structures complexes
void findPathsBFSSimple(Graph *graph, int start, int end, Path ***paths, int *pathCount, int max_capacity)
{
    (void)start;  // Éviter le warning de paramètre non utilisé
    (void)end;    // Éviter le warning de paramètre non utilisé
    
    if (*pathCount >= max_capacity || !graph) return;
    
    // Utiliser la fonction find_path existante pour trouver des chemins
    // en marquant progressivement les nœuds comme utilisés
    bool used_nodes[graph->node_count];
    
    // Initialiser le tableau des nœuds utilisés
    for (int i = 0; i < graph->node_count; i++) {
        used_nodes[i] = false;
    }
    
    // Trouver des chemins jusqu'à ce qu'il n'y en ait plus
    while (*pathCount < max_capacity && *pathCount < 50) { // Limite raisonnable
        resetNodePositions(graph);
        Path *new_path = find_path(graph, used_nodes);
        
        if (!new_path || new_path->len <= 0) {
            break; // Aucun chemin trouvé
        }
        
        // Ajouter le chemin trouvé
        (*paths)[(*pathCount)++] = new_path;
        
        // Marquer les nœuds intermédiaires comme utilisés (sauf start et end)
        for (int i = 0; i < new_path->len - 1; i++) { // Exclure le dernier (end)
            int node_index = new_path->nodes[i];
            Node *node = getNodeByIndex(graph, node_index);
            if (node && !node->isStart && !node->isEnd) {
                used_nodes[node_index] = true;
            }
        }
    }
}
