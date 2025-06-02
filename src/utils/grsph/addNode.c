#include "data.h"

static int current_pos = 0;

void resetNodePosition(void) {
    current_pos = 0;
}

void addNode(Graph* graph, char * Nan, int x , int y, bool isStart, bool isEnd) {
    if (current_pos >= graph->size) {
        return; // No more space in the graph
    }
    Node* newNode = (Node*)ft_arnalloc(sizeof(Node));
    if (!newNode) {
        return; // Memory allocation failed
    }
    newNode->Nan = Nan;
    newNode->x = x;
    newNode->y = y;
    newNode->isStart = isStart;
    newNode->isEnd = isEnd;
    newNode->next = NULL;
    graph->adjacencyList[current_pos] = newNode;
    current_pos++;
}