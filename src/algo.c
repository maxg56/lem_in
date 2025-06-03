#include "lem-in.h"

void resetNodePosition(Graph *graph) {
	for (int i = 0; i < graph->size; i++) {
		Node *node = getNodeByIndex(graph, i);
		if (node) {
			node->visited = false;
			node->parentNode = -1;
		}
	}
}

Path *find_path(Graph* graph)
{
	resetNodePosition(graph);

	Node* startNode = getStartNode(graph);
	Node* endNode = getEndNode(graph);
	if (!startNode || !endNode)
		return NULL;

	int start = findNodeByName(graph, startNode->Nan);
	int end = findNodeByName(graph, endNode->Nan);
	if (start == -1 || end == -1)
		return NULL;

	int queue[graph->size];
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
				queue[rear++] = ni;
			}
		}
	}

	if (!endNode->visited)
		return NULL;

	return build_path(graph, end);
}

