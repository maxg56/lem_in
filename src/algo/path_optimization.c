/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_optimization.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"
#include <limits.h>

// Fonction pour calculer le nombre de tours nécessaires avec un ensemble de chemins
int calculateTurns(Path **paths, int path_count, int total_ants)
{
    if (path_count == 0) return INT_MAX;
    
    // Calculer les tours avec assignation optimale
    int min_turns = INT_MAX;
    
    // Essayer différentes combinaisons d'assignation
    for (int main_path = 0; main_path < path_count; main_path++) {
        int turns = paths[main_path]->len + total_ants - 1;
        
        // Pour chemins multiples, calculer l'optimisation
        if (path_count > 1) {
            int remaining_ants = total_ants;
            int max_finish_time = 0;
            
            for (int i = 0; i < path_count && remaining_ants > 0; i++) {
                int ants_on_path = (remaining_ants + path_count - i - 1) / (path_count - i);
                int finish_time = paths[i]->len + ants_on_path - 1;
                if (finish_time > max_finish_time) {
                    max_finish_time = finish_time;
                }
                remaining_ants -= ants_on_path;
            }
            turns = max_finish_time;
        }
        
        if (turns < min_turns) {
            min_turns = turns;
        }
    }
    
    return min_turns;
}

// Vérifie si deux chemins sont disjoints (ne partagent aucun nœud intermédiaire)
bool arePathsDisjoint(Path *path1, Path *path2)
{
    // Vérifier que les chemins ne partagent aucun nœud intermédiaire
    for (int i = 0; i < path1->len - 1; i++) { // Exclure le dernier nœud (fin)
        for (int j = 0; j < path2->len - 1; j++) { // Exclure le dernier nœud (fin)
            if (path1->nodes[i] == path2->nodes[j]) {
                return false;
            }
        }
    }
    return true;
}

// Sélectionne les chemins optimaux pour minimiser le nombre de tours
Path **selectOptimalPaths(Path **all_paths, int total_count, int total_ants, int *selected_count)
{
    if (total_count == 0) {
        *selected_count = 0;
        return NULL;
    }
    
    Path **best_combination = ft_arnalloc(sizeof(Path*) * total_count);
    int best_count = 0;
    int min_turns = INT_MAX;
    
    // Limiter les combinaisons pour éviter l'explosion combinatoire
    int max_combinations = (total_count > 10) ? 1024 : (1 << total_count);
    
    // Essayer différentes combinaisons de chemins disjoints
    for (int mask = 1; mask < max_combinations; mask++) {
        Path **current_paths = ft_arnalloc(sizeof(Path*) * total_count);
        int current_count = 0;
        
        // Construire la combinaison actuelle
        for (int i = 0; i < total_count && i < 20; i++) { // Limiter à 20 chemins max
            if (mask & (1 << i)) {
                current_paths[current_count++] = all_paths[i];
            }
        }
        
        // Vérifier que tous les chemins sont disjoints
        bool all_disjoint = true;
        for (int i = 0; i < current_count - 1 && all_disjoint; i++) {
            for (int j = i + 1; j < current_count && all_disjoint; j++) {
                if (!arePathsDisjoint(current_paths[i], current_paths[j])) {
                    all_disjoint = false;
                }
            }
        }
        
        if (all_disjoint) {
            int turns = calculateTurns(current_paths, current_count, total_ants);
            if (turns < min_turns) {
                min_turns = turns;
                best_count = current_count;
                for (int i = 0; i < current_count; i++) {
                    best_combination[i] = current_paths[i];
                }
            }
        }
    }
    
    // Si aucune combinaison trouvée, prendre le plus court chemin
    if (best_count == 0) {
        best_combination[0] = all_paths[0];
        best_count = 1;
    }
    
    *selected_count = best_count;
    return best_combination;
}

// Trie les chemins par longueur (plus courts d'abord)
void sortPathsByLength(Path **paths, int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (paths[j]->len > paths[j + 1]->len) {
                Path *temp = paths[j];
                paths[j] = paths[j + 1];
                paths[j + 1] = temp;
            }
        }
    }
}

// Fonction principale pour trouver tous les chemins optimaux
Path **findAllPaths(Graph *graph, int *count)
{
    // Limiter la capacité pour éviter l'explosion sur de gros graphes
    int max_capacity = (graph->node_count > 100) ? 1000 : graph->node_count * graph->node_count;
    if (max_capacity > 10000) max_capacity = 10000; // Limite absolue
    
    Path **paths = ft_arnalloc(sizeof(Path*) * max_capacity);
    if (!paths)
        return NULL;

    int pathCount = 0;
    Node *start_node = getStartNode(graph);
    Node *end_node = getEndNode(graph);
    
    if (!start_node || !end_node) {
        *count = 0;
        return paths;
    }

    int start_index = findNodeByName(graph, start_node->Nan);
    int end_index = findNodeByName(graph, end_node->Nan);
    
    if (start_index == -1 || end_index == -1) {
        *count = 0;
        return paths;
    }
    
    // Utiliser BFS simple pour trouver tous les chemins
    findPathsBFSSimple(graph, start_index, end_index, &paths, &pathCount, max_capacity);
    
    // Trier les chemins par longueur (plus courts d'abord)
    sortPathsByLength(paths, pathCount);
    
    // Optimiser la sélection des chemins pour minimiser les tours
    Path **optimizedPaths = selectOptimalPaths(paths, pathCount, graph->nb_fourmis, count);
    
    return optimizedPaths;
}
