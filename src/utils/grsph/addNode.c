#include "data.h"

void resetNodePosition(void) {
    // Cette fonction n'est plus nécessaire avec la nouvelle approche
    // mais on la garde pour la compatibilité
}

void addNode(Graph* graph, char * Nan, int x , int y, bool isStart, bool isEnd) {
    // Redimensionner si nécessaire
    if (graph->node_count >= graph->size_max) {
        if (!resizeGraph(graph, graph->size_max * 2)) {
            return; // Failed to resize
        }
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
    graph->adjacencyList[graph->node_count] = newNode;
    graph->node_count++; // Incrémenter le nombre de nœuds utilisés
}