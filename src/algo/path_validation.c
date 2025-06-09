/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

// Fonction DFS récursive pour compatibilité avec les anciens tests
void findAllPathsRecursive(Graph *graph, int current, int target, bool *visited, 
                          int *current_path, int path_length, Path ***paths, 
                          int *pathCount, int max_capacity)
{
    // Limiter la profondeur pour éviter l'explosion combinatoire
    if (path_length > 50 || *pathCount >= 100) {
        return;
    }
    
    // Ajouter le nœud actuel au chemin
    current_path[path_length] = current;
    path_length++;
    
    // Si on a atteint la cible, créer et sauvegarder le chemin
    if (current == target) {
        if (*pathCount < max_capacity) {
            Path *new_path = ft_arnalloc(sizeof(Path));
            if (new_path) {
                new_path->len = path_length - 1; // Exclure le nœud de départ
                new_path->nodes = ft_arnalloc(sizeof(int) * new_path->len);
                if (new_path->nodes) {
                    // Copier le chemin (sans le nœud de départ)
                    for (int i = 0; i < new_path->len; i++) {
                        new_path->nodes[i] = current_path[i + 1];
                    }
                    new_path->assigned_ants = 0;
                    (*paths)[(*pathCount)++] = new_path;
                }
            }
        }
        return;
    }
    
    // Marquer le nœud actuel comme visité
    visited[current] = true;
    
    // Explorer tous les voisins non visités
    int neighbor_count = 0;
    Node **neighbors = getNeighbors(graph, current, &neighbor_count);
    
    for (int i = 0; i < neighbor_count; i++) {
        int neighbor_index = findNodeByName(graph, neighbors[i]->Nan);
        if (neighbor_index != -1 && !visited[neighbor_index]) {
            findAllPathsRecursive(graph, neighbor_index, target, visited, 
                                current_path, path_length, paths, pathCount, max_capacity);
        }
    }
    
    // Retirer le marquage pour permettre d'autres chemins (backtrack)
    visited[current] = false;
}
