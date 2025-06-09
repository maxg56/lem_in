#include "lem-in.h"
#include <limits.h>

Path *build_path(Graph *graph, int end_index)
{
	if (!graph || end_index < 0 || end_index >= graph->node_count)
		return NULL;

	int tmp[graph->node_count];
	int len = 0;
	int current_index = end_index;
	int max_iterations = graph->node_count;

	tmp[len++] = end_index;

	while (current_index != -1 && len < max_iterations)
	{
		Node* current_node = getNodeByIndex(graph, current_index);
		if (!current_node)
			break;
		if (current_node->parentNode == -1)
			break;
		current_index = current_node->parentNode;
		tmp[len++] = current_index;
	}
	if (len > 0)
		len--; // retire le nœud start du chemin
	if (len == 0)
		return NULL;
	Path *path = ft_arnalloc(sizeof(Path));
	if (!path)
		return NULL;
	path->nodes = ft_arnalloc(sizeof(int) * len);
	if (!path->nodes)
		return NULL;
	for (int i = 0; i < len; i++)// Inverser pour avoir le chemin du début vers la fin (sans start)
		path->nodes[i] = tmp[len - 1 - i];
	path->len = len;
	path->assigned_ants = 0;
	return path;
}


void antsMovements(Graph *graph, Path *path)
{
	int total = graph->nb_fourmis;
	if (!graph || !path || total <= 0 || path->len == 0) {
		return;
	}
	int cheminLength = path->len;
	int lastAntId = 0;
	int tour = 0;

	while(lastAntId < total + cheminLength - 1) {
		tour++;
		for(int i = 0; i < total ;i++)
		{
			int antId = i + 1;
			int position = tour - antId;
			if(position < 0 || position >= cheminLength)
				continue;
			if(position >= 0 && position < cheminLength)
			{
				Node *room = getNodeByIndex(graph, path->nodes[position]);
				ft_putstr_fd("L", 1);
				ft_putnbr_fd(antId, 1);
				ft_putstr_fd("-", 1);
				ft_putstr_fd(room->Nan, 1);
				ft_putstr_fd(" ", 1);
			}
		}
		ft_putchar_fd('\n', 1);
		lastAntId++;
	}
}

void	multiplePaths(Graph *graph, Path **paths, int pathCount)
{
	int totalAnts = graph->nb_fourmis;
	int antsArrived = 0;
	t_list *activeAnts = NULL;
	
	// Assigner les fourmis de manière optimale
	assignAnts(paths, pathCount, totalAnts);
	
	// Tableau pour suivre le nombre de fourmis déjà envoyées sur chaque chemin
	int antsSent[pathCount];
	for (int i = 0; i < pathCount; i++) {
		antsSent[i] = 0;
	}
	
	int nextAntId = 1;
	int turn = 0;
	
	while (antsArrived < totalAnts || activeAnts != NULL)
	{
		turn++;
		bool anyMovement = false;
		
		// Tableau pour suivre les positions occupées à ce tour
		bool *nodeOccupied = ft_arnalloc(sizeof(bool) * graph->node_count);
		for (int i = 0; i < graph->node_count; i++) {
			nodeOccupied[i] = false;
		}
		
		// Première passe : déplacer les fourmis existantes
		t_list *current = activeAnts;
		t_list *toRemove = NULL;
		
		while (current)
		{
			Ants *ant = current->content;
			
			// Vérifier si la fourmi peut avancer
			if (ant->position + 1 < paths[ant->path_index]->len) {
				int nextNodeIndex = paths[ant->path_index]->nodes[ant->position + 1];
				Node *nextNode = getNodeByIndex(graph, nextNodeIndex);
				
				// Vérifier si la position suivante est libre (l'end node peut toujours accepter des fourmis)
				if (!nodeOccupied[nextNodeIndex] || nextNode->isEnd) {
					ant->position++;
					// Ne pas marquer l'end node comme occupé car les fourmis disparaissent
					if (!nextNode->isEnd) {
						nodeOccupied[nextNodeIndex] = true;
					}
					
					Node *room = getNodeByIndex(graph, nextNodeIndex);
					ft_printf("L%d-%s ", ant->id, room->Nan);
					anyMovement = true;
				}
			} else {
				// La fourmi a atteint la fin
				antsArrived++;
				
				// Marquer cette fourmi pour suppression
				t_list *nodeToRemove = ft_lstnew(current);
				ft_lstadd_back(&toRemove, nodeToRemove);
			}
			
			current = current->next;
		}
		
		// Supprimer les fourmis arrivées
		while (toRemove) {
			t_list *antNode = (t_list*)toRemove->content;
			
			// Trouver et supprimer le nœud de la liste des fourmis actives
			t_list *prev = NULL;
			t_list *searchCurrent = activeAnts;
			
			while (searchCurrent && searchCurrent != antNode) {
				prev = searchCurrent;
				searchCurrent = searchCurrent->next;
			}
			
			if (searchCurrent) {
				if (prev) {
					prev->next = searchCurrent->next;
				} else {
					activeAnts = searchCurrent->next;
				}
			}
			
			t_list *toRemoveNext = toRemove->next;
			toRemove = toRemoveNext;
		}
		
		// Deuxième passe : envoyer de nouvelles fourmis
		for (int i = 0; i < pathCount && nextAntId <= totalAnts; i++)
		{
			if (antsSent[i] < paths[i]->assigned_ants) {
				int startNodeIndex = paths[i]->nodes[0];
				
				// Vérifier si le premier nœud du chemin est libre
				if (!nodeOccupied[startNodeIndex]) {
					Ants *newAnt = ft_arnalloc(sizeof(Ants));
					newAnt->id = nextAntId++;
					newAnt->path_index = i;
					newAnt->position = 0;
					newAnt->arrived = false;
					
					ft_lstadd_back(&activeAnts, ft_lstnew(newAnt));
					antsSent[i]++;
					
					nodeOccupied[startNodeIndex] = true;
					Node *room = getNodeByIndex(graph, startNodeIndex);
					ft_printf("L%d-%s ", newAnt->id, room->Nan);
					anyMovement = true;
				}
			}
		}
		
		if (anyMovement) {
			ft_putchar_fd('\n', 1);
		}
		
		// Sécurité pour éviter les boucles infinies
		if (turn > totalAnts + pathCount + 10) {
			break;
		}
		
		// Si toutes les fourmis sont arrivées et qu'il n'y a plus de fourmis actives, on peut arrêter
		if (antsArrived >= totalAnts && activeAnts == NULL) {
			break;
		}
	}
}

void assignAnts(Path **paths, int path_count, int total_ants)
{
    if (path_count == 0 || total_ants == 0) return;
    
    if (path_count == 1) {
        paths[0]->assigned_ants = total_ants;
        return;
    }

    // Réinitialiser les assignations
    for (int i = 0; i < path_count; i++) {
        paths[i]->assigned_ants = 0;
    }

    // Assignation optimale pour minimiser le nombre de tours
    // Algorithme simple mais efficace : distribution équilibrée en tenant compte des longueurs
    int min_turns = INT_MAX;
    int best_assignment[path_count];
    
    // Initialiser avec une répartition simple
    for (int i = 0; i < path_count; i++) {
        best_assignment[i] = total_ants / path_count;
    }
    
    // Distribuer le reste
    int remainder = total_ants % path_count;
    for (int i = 0; i < remainder; i++) {
        best_assignment[i]++;
    }
    
    // Essayer d'optimiser en redistribuant les fourmis
    for (int attempt = 0; attempt < path_count; attempt++) {
        int assignment[path_count];
        for (int i = 0; i < path_count; i++) assignment[i] = 0;
        
        int remaining_ants = total_ants;
        
        // Distribuer les fourmis de manière cyclique en commençant par les chemins courts
        while (remaining_ants > 0) {
            int best_path = 0;
            
            // Trouver le chemin avec le meilleur ratio (temps de fin le plus petit)
            for (int i = 1; i < path_count; i++) {
                int current_finish_i = (assignment[i] > 0) ? paths[i]->len + assignment[i] - 1 : paths[i]->len;
                int current_finish_best = (assignment[best_path] > 0) ? paths[best_path]->len + assignment[best_path] - 1 : paths[best_path]->len;
                
                if (current_finish_i < current_finish_best) {
                    best_path = i;
                }
            }
            
            assignment[best_path]++;
            remaining_ants--;
        }
        
        // Calculer le nombre de tours pour cette répartition
        int max_finish_time = 0;
        for (int i = 0; i < path_count; i++) {
            if (assignment[i] > 0) {
                int finish_time = paths[i]->len + assignment[i] - 1;
                if (finish_time > max_finish_time) {
                    max_finish_time = finish_time;
                }
            }
        }
        
        if (max_finish_time < min_turns) {
            min_turns = max_finish_time;
            for (int i = 0; i < path_count; i++) {
                best_assignment[i] = assignment[i];
            }
        }
    }
    
    // Appliquer la meilleure assignation
    for (int i = 0; i < path_count; i++) {
        paths[i]->assigned_ants = best_assignment[i];
    }
}
