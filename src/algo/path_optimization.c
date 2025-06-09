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
    
    if (path_count == 1) {
        return paths[0]->len + total_ants - 1;
    }
    
    // Simuler l'assignation optimale (même logique que assignAnts)
    int assignment[path_count];
    for (int i = 0; i < path_count; i++) {
        assignment[i] = 0;
    }
    
    // Assigner une fourmi à la fois au chemin qui minimise le temps de fin
    for (int ant = 0; ant < total_ants; ant++) {
        int best_path = 0;
        int min_finish_time = INT_MAX;
        
        for (int i = 0; i < path_count; i++) {
            int finish_time = paths[i]->len + assignment[i];
            if (finish_time < min_finish_time) {
                min_finish_time = finish_time;
                best_path = i;
            }
        }
        
        assignment[best_path]++;
    }
    
    // Calculer le temps de fin maximum
    int max_finish_time = 0;
    for (int i = 0; i < path_count; i++) {
        if (assignment[i] > 0) {
            int finish_time = paths[i]->len + assignment[i] - 1;
            if (finish_time > max_finish_time) {
                max_finish_time = finish_time;
            }
        }
    }
    
    return max_finish_time;
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
// Optimisé pour réduire le nombre de lignes de sortie en testant
// différentes combinaisons de chemins, même non-disjoints
Path **selectOptimalPaths(Path **all_paths, int total_count, int total_ants, int *selected_count)
{
    if (total_count == 0) {
        *selected_count = 0;
        return NULL;
    }
    
    Path **best_combination = ft_arnalloc(sizeof(Path*) * total_count);
    int best_count = 1;
    
    // 1. Commencer avec le chemin le plus court seul
    best_combination[0] = all_paths[0];
    int min_turns = all_paths[0]->len + total_ants - 1;
    
    // 2. Tester des combinaisons de chemins (même non-disjoints)
    // La fonction calculateTurnsForMultiplePaths gère les collisions
    if (total_count >= 2) {
        // Stratégie optimisée: tester différentes combinaisons de chemins
        // Pour big-superposition, nous devons explorer plus de combinaisons
        int max_paths_to_test = (total_count > 15) ? 15 : total_count;
        
        // Test 1: Combinaisons séquentielles (premiers N chemins)
        for (int num_paths = 2; num_paths <= max_paths_to_test; num_paths++) {
            Path **test_paths = ft_arnalloc(sizeof(Path*) * num_paths);
            
            // Prendre les N chemins les plus courts
            for (int i = 0; i < num_paths; i++) {
                test_paths[i] = all_paths[i];
            }
            
            int turns = calculateTurnsForMultiplePaths(test_paths, num_paths, total_ants);
            
            if (turns < min_turns) {
                min_turns = turns;
                best_count = num_paths;
                for (int i = 0; i < num_paths; i++) {
                    best_combination[i] = test_paths[i];
                }
            }
        }
        
        // Test 2: Combinaisons alternatives pour cartes avec superposition
        if (total_count >= 4) {
            // Tester différentes combinaisons de chemins espacés
            for (int spacing = 2; spacing <= 3 && spacing < total_count; spacing++) {
                for (int start = 0; start + spacing < total_count; start++) {
                    int num_paths = 0;
                    Path **test_paths = ft_arnalloc(sizeof(Path*) * max_paths_to_test);
                    
                    // Sélectionner des chemins avec espacement pour diversité
                    for (int i = start; i < total_count && num_paths < max_paths_to_test; i += spacing) {
                        test_paths[num_paths++] = all_paths[i];
                        if (num_paths >= 8) break; // Limiter pour éviter l'explosion
                    }
                    
                    if (num_paths >= 2) {
                        int turns = calculateTurnsForMultiplePaths(test_paths, num_paths, total_ants);
                        
                        if (turns < min_turns) {
                            min_turns = turns;
                            best_count = num_paths;
                            for (int i = 0; i < num_paths; i++) {
                                best_combination[i] = test_paths[i];
                            }
                        }
                    }
                }
            }
        }
        
        // Test 3: Combinaisons optimisées par longueur et stratégie glouttonne
        if (total_count >= 3) {
            // Stratégie 1: Le plus court + chemins de longueurs similaires
            Path **test_paths = ft_arnalloc(sizeof(Path*) * max_paths_to_test);
            test_paths[0] = all_paths[0]; // Le plus court
            int num_paths = 1;
            
            // Ajouter des chemins de longueurs variées
            for (int i = 1; i < total_count && num_paths < max_paths_to_test; i++) {
                if (all_paths[i]->len <= all_paths[0]->len + 3) { // Pas trop longs
                    test_paths[num_paths++] = all_paths[i];
                    if (num_paths >= 10) break;
                }
            }
            
            if (num_paths >= 2) {
                int turns = calculateTurnsForMultiplePaths(test_paths, num_paths, total_ants);
                
                if (turns < min_turns) {
                    min_turns = turns;
                    best_count = num_paths;
                    for (int i = 0; i < num_paths; i++) {
                        best_combination[i] = test_paths[i];
                    }
                }
            }
            
            // Stratégie 2: Recherche glouttonne pour cartes très complexes
            if (total_ants > 50 && total_count > 10) {
                // Pour de nombreuses fourmis, essayer une approche plus agressive
                int max_greedy = (total_count > 20) ? 20 : total_count;
                Path **greedy_paths = ft_arnalloc(sizeof(Path*) * max_greedy);
                int greedy_count = 0;
                
                // Prendre tous les chemins de longueur raisonnable
                for (int i = 0; i < total_count && greedy_count < max_greedy; i++) {
                    if (all_paths[i]->len <= all_paths[0]->len + 5) {
                        greedy_paths[greedy_count++] = all_paths[i];
                    }
                }
                
                if (greedy_count >= 2) {
                    int turns = calculateTurnsForMultiplePaths(greedy_paths, greedy_count, total_ants);
                    
                    if (turns < min_turns) {
                        min_turns = turns;
                        best_count = greedy_count;
                        for (int i = 0; i < greedy_count; i++) {
                            best_combination[i] = greedy_paths[i];
                        }
                    }
                }
            }
        }
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

// Calcule le nombre de tours pour des chemins multiples (même non-disjoints)
int calculateTurnsForMultiplePaths(Path **paths, int path_count, int total_ants)
{
    if (path_count == 0) return INT_MAX;
    if (path_count == 1) return paths[0]->len + total_ants - 1;
    
    // Utiliser la même logique que calculateTurns et assignAnts
    int assignment[path_count];
    for (int i = 0; i < path_count; i++) {
        assignment[i] = 0;
    }
    
    // Assigner une fourmi à la fois au chemin qui minimise le temps de fin
    for (int ant = 0; ant < total_ants; ant++) {
        int best_path = 0;
        int min_finish_time = INT_MAX;
        
        for (int i = 0; i < path_count; i++) {
            int finish_time = paths[i]->len + assignment[i];
            if (finish_time < min_finish_time) {
                min_finish_time = finish_time;
                best_path = i;
            }
        }
        
        assignment[best_path]++;
    }
    
    // Calculer le temps de fin maximum
    int max_finish_time = 0;
    for (int i = 0; i < path_count; i++) {
        if (assignment[i] > 0) {
            int finish_time = paths[i]->len + assignment[i] - 1;
            if (finish_time > max_finish_time) {
                max_finish_time = finish_time;
            }
        }
    }
    
    return max_finish_time;
}

// Trouve le meilleur ensemble de chemins disjoints de taille donnée
bool findBestDisjointSet(Path **all_paths, int total_count, int target_size, Path **result_set)
{
    if (target_size <= 0 || total_count < target_size) return false;
    
    // Pour de petits ensembles, utiliser une recherche exhaustive simple
    if (target_size <= 3 && total_count <= 10) {
        int best_total_length = INT_MAX;
        bool found = false;
        
        // Générer toutes les combinaisons possibles
        for (int mask = 0; mask < (1 << total_count); mask++) {
            if (__builtin_popcount(mask) != target_size) continue;
            
            Path *candidate_set[target_size];
            int set_index = 0;
            
            // Construire l'ensemble candidat
            for (int i = 0; i < total_count && set_index < target_size; i++) {
                if (mask & (1 << i)) {
                    candidate_set[set_index++] = all_paths[i];
                }
            }
            
            // Vérifier que tous les chemins sont disjoints
            bool all_disjoint = true;
            for (int i = 0; i < target_size - 1 && all_disjoint; i++) {
                for (int j = i + 1; j < target_size && all_disjoint; j++) {
                    if (!arePathsDisjoint(candidate_set[i], candidate_set[j])) {
                        all_disjoint = false;
                    }
                }
            }
            
            if (all_disjoint) {
                // Calculer la qualité de cet ensemble (somme des longueurs)
                int total_length = 0;
                for (int i = 0; i < target_size; i++) {
                    total_length += candidate_set[i]->len;
                }
                
                if (total_length < best_total_length) {
                    best_total_length = total_length;
                    for (int i = 0; i < target_size; i++) {
                        result_set[i] = candidate_set[i];
                    }
                    found = true;
                }
            }
        }
        
        return found;
    }
    
    // Pour des ensembles plus grands, utiliser un algorithme glouton
    bool used[total_count];
    for (int i = 0; i < total_count; i++) {
        used[i] = false;
    }
    
    int selected_count = 0;
    
    for (int iter = 0; iter < target_size; iter++) {
        int best_candidate = -1;
        
        // Trouver le meilleur chemin non utilisé qui est disjoint des sélectionnés
        for (int i = 0; i < total_count; i++) {
            if (used[i]) continue;
            
            bool is_disjoint = true;
            for (int j = 0; j < selected_count && is_disjoint; j++) {
                if (!arePathsDisjoint(all_paths[i], result_set[j])) {
                    is_disjoint = false;
                }
            }
            
            if (is_disjoint) {
                if (best_candidate == -1 || all_paths[i]->len < all_paths[best_candidate]->len) {
                    best_candidate = i;
                }
            }
        }
        
        if (best_candidate != -1) {
            result_set[selected_count++] = all_paths[best_candidate];
            used[best_candidate] = true;
        } else {
            return false; // Impossible de trouver assez de chemins disjoints
        }
    }
    
    return selected_count == target_size;
}
