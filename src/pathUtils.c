#include "lem-in.h"

Path *build_path(Graph *graph, int end_index)
{
	if (!graph || end_index < 0 || end_index >= graph->node_count)
		return NULL;

	int tmp[graph->node_count];
	int len = 0;
	int current_index = end_index;
	int max_iterations = graph->node_count;

	// Ajouter d'abord le nœud de fin
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

	// Inverser pour avoir le chemin du début vers la fin (sans start)
	for (int i = 0; i < len; i++)
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