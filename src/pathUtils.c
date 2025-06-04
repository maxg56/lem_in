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
	while (antsArrived < totalAnts)
	{
		t_list *tmp = activeAnts;
		while (tmp)
		{
			Ants *a = tmp->content;
			a->position++;
			if(a->position < paths[a->path_index]->len)
			{
				Node *room = getNodeByIndex(graph, paths[a->path_index]->nodes[a->position]);
				ft_printf("L%d-%s ", a->id, room->Nan);
			}
			else
				antsArrived++;
			tmp = tmp->next;
		}
		for (int i = 0; i < pathCount && nextAntId <= totalAnts; i++)
		{
			if(paths[i]->assigned_ants > 0)
			{
				Ants *newAnt = ft_arnalloc(sizeof(Ants));
				newAnt->id = nextAntId++;
				newAnt->path_index = i;
				newAnt->position = 0;
				ft_lstadd_back(&activeAnts, ft_lstnew(newAnt));
				paths[i]->assigned_ants--;
				Node *room = getNodeByIndex(graph, paths[i]->nodes[0]);
				ft_printf("L%d-%s ", newAnt->id, room->Nan);
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
