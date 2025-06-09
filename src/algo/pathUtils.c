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
	
	// Don't remove the start node - include it in the path
	if (len == 0)
		return NULL;
	Path *path = ft_arnalloc(sizeof(Path));
	if (!path)
		return NULL;
	path->nodes = ft_arnalloc(sizeof(int) * len);
	if (!path->nodes)
		return NULL;
	for (int i = 0; i < len; i++) // Inverser pour avoir le chemin du début vers la fin (avec start)
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
		t_list *prev = NULL;
		
		while (current)
		{
			Ants *ant = current->content;
			t_list *next = current->next;
			
			// Vérifier si la fourmi peut avancer
			if (ant->position + 1 < paths[ant->path_index]->len) {
				int nextNodeIndex = paths[ant->path_index]->nodes[ant->position + 1];
				Node *nextNode = getNodeByIndex(graph, nextNodeIndex);
				
				// Vérifier si la position suivante est libre (l'end node peut toujours accepter des fourmis)
				// Le nœud de départ (start) ne doit pas être bloqué car les fourmis en sortent
				if (!nodeOccupied[nextNodeIndex] || nextNode->isEnd || nextNode->isStart) {
					ant->position++;
					// Ne pas marquer l'end node ou start node comme occupé
					if (!nextNode->isEnd && !nextNode->isStart) {
						nodeOccupied[nextNodeIndex] = true;
					}
					
					// Afficher le mouvement seulement si la fourmi QUITTE le start node
					// Les fourmis passent de position -1 à position 0 (start) sans affichage
					// Puis affichent tous leurs mouvements suivants, y compris vers l'end node
					if (ant->position > 0) {
						Node *room = getNodeByIndex(graph, nextNodeIndex);
						ft_printf("L%d-%s ", ant->id, room->Nan);
						anyMovement = true;
					}
					
					prev = current;
				} else {
					prev = current;
				}
			} else {
				// La fourmi a atteint la fin
				antsArrived++;
				
				// Supprimer cette fourmi de la liste
				if (prev) {
					prev->next = next;
				} else {
					activeAnts = next;
				}
				current = next;
				continue;
			}
			
			current = next;
		}
		
		// Deuxième passe : envoyer de nouvelles fourmis
		for (int i = 0; i < pathCount && nextAntId <= totalAnts; i++)
		{
			if (antsSent[i] < paths[i]->assigned_ants) {
				// Le nœud de départ ne peut jamais être bloqué car c'est le point d'origine
				// Les fourmis commencent à position -1 et vont vers position 0 (start node)
				Ants *newAnt = ft_arnalloc(sizeof(Ants));
				newAnt->id = nextAntId++;
				newAnt->path_index = i;
				newAnt->position = -1; // Commencer AVANT le chemin
				newAnt->arrived = false;
				
				ft_lstadd_back(&activeAnts, ft_lstnew(newAnt));
				antsSent[i]++;
				// Don't set anyMovement here - ants starting at position -1 don't produce visible output
			}
		}
		
		if (anyMovement) {
			ft_putchar_fd('\n', 1);
		}
		
		// Si toutes les fourmis sont arrivées, on peut arrêter immédiatement
		if (antsArrived >= totalAnts) {
			break;
		}
		
		// Sécurité pour éviter les boucles infinies - augmenter significativement la limite
		// Pour tenir compte des chemins longs (ex: 29 salles + temps pour toutes les fourmis)
		int maxTurns = totalAnts + (pathCount > 0 ? paths[0]->len * 2 : 50) + 20;
		if (turn > maxTurns) {
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

    // Algorithme d'assignation optimale amélioré
    // Pour de gros nombres de fourmis, utiliser une approche plus mathématique
    
    if (total_ants >= 50) {
        // Algorithme optimal pour gros nombres : équilibrer les temps de fin
        int min_turns = INT_MAX;
        int best_assignment[path_count];
        
        // Essayer différentes distributions en commençant par les chemins courts
        for (int start_path = 0; start_path < path_count; start_path++) {
            int assignment[path_count];
            for (int i = 0; i < path_count; i++) assignment[i] = 0;
            
            int remaining_ants = total_ants;
            
            // Distribuer les fourmis en minimisant le temps de fin maximum
            while (remaining_ants > 0) {
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
                remaining_ants--;
            }
            
            // Calculer le temps de fin maximum pour cette distribution
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
    } else {
        // Algorithme simple pour petits nombres
        for (int ant = 0; ant < total_ants; ant++) {
            int best_path = 0;
            int min_finish_time = INT_MAX;
            
            for (int i = 0; i < path_count; i++) {
                int finish_time = paths[i]->len + paths[i]->assigned_ants;
                if (finish_time < min_finish_time) {
                    min_finish_time = finish_time;
                    best_path = i;
                }
            }
            
            paths[best_path]->assigned_ants++;
        }
    }
}
