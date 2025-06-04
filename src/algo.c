#include "lem-in.h"

void resetNodePositions(Graph *graph) {
	if (!graph) return;
	for (int i = 0; i < graph->node_count; i++) {
		Node *node = getNodeByIndex(graph, i);
		if (node) {
			node->visited = false;
			node->parentNode = -1;
		}
	}
}

Path *find_path(Graph* graph)
{
	resetNodePositions(graph);

	Node* startNode = getStartNode(graph);
	Node* endNode = getEndNode(graph);
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
		Node** neighbors = getNeighbors(graph, current_index, &count);
		for (int i = 0; i < count; i++)
		{
			int ni = findNodeByName(graph, neighbors[i]->Nan);
			if (ni != -1 && !neighbors[i]->visited)
			{
				neighbors[i]->visited = true;
				neighbors[i]->parentNode = current_index;
				if (rear < graph->node_count) { // Prevent queue overflow
					queue[rear++] = ni;
				}
			}
		}
	}

	if (!endNode->visited)
		return NULL;

	return build_path(graph, end);
}

Path **findAllPaths(Graph* graph, int *count) {
	int max_capacity = graph->node_count * 2;
	Path **paths = ft_arnalloc(sizeof(Path*) * max_capacity);
	if (!paths) return NULL;

	int pathCount = 0;

	// 🔍 Connexions à start et end
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

		max_paths = (start_conn < end_conn) ? start_conn : end_conn + 1;
	}

	while (1)
	{
		if (pathCount >= max_paths || pathCount >= max_capacity)
			break;

		Path *path = find_path(graph);
		if (!path)
			break;

		paths[pathCount++] = path;

		// Supprimer les liens internes (sauf start/end)
		for (int i = 0; i < path->len - 1; i++)
		{
			Node *a = getNodeByIndex(graph, path->nodes[i]);
			Node *b = getNodeByIndex(graph, path->nodes[i + 1]);
			if (!a->isStart && !b->isEnd)
				removeEdge(graph, path->nodes[i], path->nodes[i + 1]);
		}
		resetNodePositions(graph);
	}

	*count = pathCount;
	return paths;
}

