#include "lem-in.h"
#include <limits.h>

void resetNodePositions(Graph *graph)
{
    if (!graph) return;
    for (int i = 0; i < graph->node_count; i++)
    {
        Node *node = getNodeByIndex(graph, i);
        if (node)
        {
            node->visited = false;
            node->parentNode = -1;
            node->antCount = 0; // Ajout pour suivre le nombre de fourmis dans chaque nœud
        }
    }
}

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

void moveAnts(Graph *graph, Path **paths, int pathCount)
{
    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);

    if (!startNode || !endNode)
        return;

    // Déplacer les fourmis le long des chemins
    for (int i = 0; i < pathCount; i++)
    {
        Path *path = paths[i];
        for (int j = path->len - 1; j > 0; j--) // Parcourir le chemin à l'envers
        {
            Node *currentNode = getNodeByIndex(graph, path->nodes[j]);
            Node *previousNode = getNodeByIndex(graph, path->nodes[j - 1]);

            if (previousNode->antCount > 0) // Si une fourmi est présente dans le nœud précédent
            {
                currentNode->antCount++; // Ajouter une fourmi au nœud actuel
                previousNode->antCount--; // Retirer une fourmi du nœud précédent

                // Si la fourmi atteint le nœud final, elle est retirée du graphe
                if (currentNode == endNode)
                    currentNode->antCount--;
            }
        }
    }

    // Ajouter une nouvelle fourmi au départ si possible
    if (startNode->antCount > 0)
    {
        for (int i = 0; i < pathCount; i++)
        {
            Path *path = paths[i];
            Node *nextNode = getNodeByIndex(graph, path->nodes[1]);
            if (nextNode->antCount == 0) // Vérifier si le prochain nœud est libre
            {
                nextNode->antCount++;
                startNode->antCount--;
                break; // Une seule fourmi quitte le départ par tour
            }
        }
    }
}

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

    // Utiliser DFS récursif pour trouver tous les chemins
    bool visited[graph->node_count];
    int current_path[graph->node_count];
    int path_length = 0;
    
    // Initialiser les tableaux
    for (int i = 0; i < graph->node_count; i++) {
        visited[i] = false;
        current_path[i] = -1;
    }
    
    int start_index = findNodeByName(graph, start_node->Nan);
    int end_index = findNodeByName(graph, end_node->Nan);
    
    if (start_index == -1 || end_index == -1) {
        *count = 0;
        return paths;
    }
    
    findAllPathsRecursive(graph, start_index, end_index, visited, current_path, 
                         path_length, &paths, &pathCount, max_capacity);
    
    // Trier les chemins par longueur (plus courts d'abord)
    sortPathsByLength(paths, pathCount);
    
    // Optimiser la sélection des chemins pour minimiser les tours
    Path **optimizedPaths = selectOptimalPaths(paths, pathCount, graph->nb_fourmis, count);
    
    return optimizedPaths;
}