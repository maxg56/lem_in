#include "lem-in.h"

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
	int nextAntId = 1;
	int antsArrived = 0;
	t_list *activeAnts = NULL;
	
	// Tableau pour suivre les positions occupées à chaque tour
	bool *nodeOccupied = ft_arnalloc(sizeof(bool) * graph->node_count);
	
	while (antsArrived < totalAnts)
	{
		// Réinitialiser les positions occupées
		for (int i = 0; i < graph->node_count; i++) {
			nodeOccupied[i] = false;
		}
		
		// Marquer le noeud de départ comme occupé s'il y a des fourmis en attente
		Node *startNode = getStartNode(graph);
		int startIndex = findNodeByName(graph, startNode->Nan);
		if (startIndex != -1) {
			nodeOccupied[startIndex] = true;
		}
		
		// Première passe : déplacer les fourmis existantes
		t_list *tmp = activeAnts;
		t_list *toRemove = NULL;
		
		while (tmp)
		{
			Ants *ant = tmp->content;
			bool antMoved = false;
			
			// Vérifier si la fourmi peut avancer
			if (ant->position + 1 < paths[ant->path_index]->len) {
				int nextPosition = paths[ant->path_index]->nodes[ant->position + 1];
				
				// Vérifier si la position suivante est libre
				if (!nodeOccupied[nextPosition]) {
					ant->position++;
					nodeOccupied[nextPosition] = true;
					antMoved = true;
					
					Node *room = getNodeByIndex(graph, nextPosition);
					ft_printf("L%d-%s ", ant->id, room->Nan);
				}
			} else {
				// La fourmi a atteint la fin
				antsArrived++;
				antMoved = true;
				
				// Marquer cette fourmi pour suppression
				t_list *nodeToRemove = ft_lstnew(tmp);
				ft_lstadd_back(&toRemove, nodeToRemove);
			}
			
			// Si la fourmi n'a pas bougé, elle reste à sa position actuelle
			if (!antMoved && ant->position < paths[ant->path_index]->len) {
				int currentPosition = paths[ant->path_index]->nodes[ant->position];
				nodeOccupied[currentPosition] = true;
			}
			
			tmp = tmp->next;
		}
		
		// Supprimer les fourmis arrivées
		while (toRemove) {
			t_list *antNode = (t_list*)toRemove->content;
			t_list *prev = NULL;
			t_list *current = activeAnts;
			
			// Trouver et supprimer le nœud
			while (current && current != antNode) {
				prev = current;
				current = current->next;
			}
			
			if (current) {
				if (prev) {
					prev->next = current->next;
				} else {
					activeAnts = current->next;
				}
			}
			
			t_list *toRemoveNext = toRemove->next;
			toRemove = toRemoveNext;
		}
		
		// Deuxième passe : ajouter de nouvelles fourmis si possible
		for (int i = 0; i < pathCount && nextAntId <= totalAnts; i++)
		{
			if (paths[i]->assigned_ants > 0)
			{
				// Vérifier si la première position du chemin est libre
				int firstPosition = paths[i]->nodes[0];
				if (!nodeOccupied[firstPosition])
				{
					Ants *newAnt = ft_arnalloc(sizeof(Ants));
					newAnt->id = nextAntId++;
					newAnt->path_index = i;
					newAnt->position = 0;
					newAnt->arrived = false;
					
					ft_lstadd_back(&activeAnts, ft_lstnew(newAnt));
					paths[i]->assigned_ants--;
					nodeOccupied[firstPosition] = true;
					
					Node *room = getNodeByIndex(graph, firstPosition);
					ft_printf("L%d-%s ", newAnt->id, room->Nan);
				}
			}
		}
		ft_putchar_fd('\n', 1);
	}
}

void assignAnts(Path **paths, int path_count, int total_ants)
{
    if (path_count == 0 || total_ants == 0) return;
    
    if (path_count == 1) {
        paths[0]->assigned_ants = total_ants;
        return;
    }
    
    int lengths[path_count];
    int total_length = 0;
    
    for (int i = 0; i < path_count; i++)
    {
        lengths[i] = paths[i]->len;
        total_length += paths[i]->len;
    }

    for (int i = 0; i < path_count; i++)
    {
        paths[i]->assigned_ants = (total_ants * (total_length - lengths[i])) / (total_length * (path_count - 1));
    }

    int assigned = 0;
    for (int i = 0; i < path_count; i++)
        assigned += paths[i]->assigned_ants;

    for (int i = 0; assigned < total_ants; i = (i + 1) % path_count)
    {
        paths[i]->assigned_ants++;
        assigned++;
    }
}
