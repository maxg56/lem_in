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
