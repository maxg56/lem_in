#include "lem-in.h"

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
		}
	}
}

Path *find_path(Graph *graph, bool *used_nodes)
{
	resetNodePositions(graph);

	Node *startNode = getStartNode(graph);
	Node *endNode = getEndNode(graph);
	if (!startNode || !endNode)
		return NULL;
	
	int start = findNodeByName(graph, startNode->Nan);
	int end = findNodeByName(graph, endNode->Nan);
	if (start == -1 || end == -1)
		return NULL;

	int queue[graph->node_count];
	int front = 0, rear = 0;
	queue[rear++] = start;
	startNode->visited = true;
	startNode->parentNode = -1;

	while (front < rear)
	{
		int current_index = queue[front++];
		if (current_index == end)
			break;
		
		int count = 0;
		Node **neighbors = getNeighbors(graph, current_index, &count);
		for (int i = 0; i < count; i++)
		{
			int ni = findNodeByName(graph, neighbors[i]->Nan);
			if (ni != -1 && !neighbors[i]->visited)
			{
				bool can_use = true;
				if (!neighbors[i]->isStart && !neighbors[i]->isEnd && used_nodes[ni])
					can_use = false;
					
				if (can_use)
				{
					neighbors[i]->visited = true;
					neighbors[i]->parentNode = current_index;
					if (rear < graph->node_count)
						queue[rear++] = ni;
				}
			}
		}
	}

	if (!endNode->visited)
		return NULL;

	return build_path(graph, end);
}

Path **findAllPaths(Graph *graph, int *count)
{
	int max_capacity = graph->node_count * 2;
	Path **paths = ft_arnalloc(sizeof(Path*) * max_capacity);
	if (!paths)
		return NULL;

	bool used_nodes[graph->node_count];
	for (int i = 0; i < graph->node_count; i++)
		used_nodes[i] = false;

	int pathCount = 0;

	Node *start_node = getStartNode(graph);
	Node *end_node = getEndNode(graph);
	int max_paths = 0;

	if (start_node && end_node)
	{
		int start_index = findNodeByName(graph, start_node->Nan);
		int end_index = findNodeByName(graph, end_node->Nan);
		int start_conn = 0, end_conn = 0;

		getNeighbors(graph, start_index, &start_conn);
		getNeighbors(graph, end_index, &end_conn);

		max_paths = (start_conn < end_conn) ? start_conn : end_conn;
	}
	int attempt = 0;
	int max_attempts = max_paths + 2;
	while (pathCount < max_paths && attempt < max_attempts)
	{
		ft_printf("\n--- Tentative %d ---\n", attempt + 1);
		
		ft_printf("Nœuds utilisés: ");
		for (int i = 0; i < graph->node_count; i++) {
			if (used_nodes[i]) {
				Node *node = getNodeByIndex(graph, i);
				ft_printf("%s ", node->Nan);
			}
		}
		ft_printf("\n");
		
		Path *path = find_path(graph, used_nodes);
		if (!path) {
			ft_printf("Aucun chemin trouvé, arrêt.\n");
			break;
		}
		bool is_duplicate = false;
		for (int existing = 0; existing < pathCount; existing++) {
			if (paths[existing]->len == path->len) {
				bool same = true;
				for (int j = 0; j < path->len; j++) {
					if (paths[existing]->nodes[j] != path->nodes[j]) {
						same = false;
						break;
					}
				}
				if (same) {
					is_duplicate = true;
					ft_printf("Chemin doublon détecté, ignoré.\n");
					break;
				}
			}
		}
		
		if (is_duplicate)
			break;
		for (int j = 1; j < path->len - 1; j++)
			used_nodes[path->nodes[j]] = true;

		paths[pathCount++] = path;
		
		ft_printf("Chemin %d accepté: ", pathCount);
		for (int k = 0; k < path->len; k++) {
			Node *node = getNodeByIndex(graph, path->nodes[k]);
			ft_printf("%s", node->Nan);
			if (k < path->len - 1) ft_printf(" -> ");
		}
		ft_printf(" (longueur: %d)\n", path->len);
		
		resetNodePositions(graph);
		attempt++;
	}
	
	*count = pathCount;
	return paths;
}
