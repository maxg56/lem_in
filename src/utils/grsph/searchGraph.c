#include "data.h"

int findNodeByName(Graph* graph, char* name) {
    if (!graph || !name) return -1;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacencyList[i] && graph->adjacencyList[i]->Nan) {
            if (ft_strcmp(graph->adjacencyList[i]->Nan, name) == 0) {
                return i;
            }
        }
    }
    
    return -1;
}

Node* getNodeByIndex(Graph* graph, int index) {
    if (!graph || index < 0 || index >= graph->node_count) {
        return NULL;
    }
    
    return graph->adjacencyList[index];
}

Node* getStartNode(Graph* graph) {
    if (!graph) return NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacencyList[i] && graph->adjacencyList[i]->isStart) {
            return graph->adjacencyList[i];
        }
    }
    
    return NULL;
}

Node* getEndNode(Graph* graph) {
    if (!graph) return NULL;
    
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacencyList[i] && graph->adjacencyList[i]->isEnd) {
            return graph->adjacencyList[i];
        }
    }
    
    return NULL;
}

int getNodeCount(Graph* graph) {
    if (!graph) return 0;
    
    return graph->node_count;
}

Node** getNeighbors(Graph* graph, int nodeIndex, int* count) {
    if (!graph || nodeIndex < 0 || nodeIndex >= graph->node_count || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    *count = 0;
    
    if (!graph->adjacencyList[nodeIndex]) {
        return NULL;
    }
    
    // Compter d'abord les voisins
    Node* current = graph->adjacencyList[nodeIndex]->next;
    while (current) {
        (*count)++;
        current = current->next;
    }
    
    if (*count == 0) {
        return NULL;
    }
    
    // Allouer le tableau de voisins
    Node** neighbors = (Node**)ft_arnalloc(sizeof(Node*) * (*count));
    if (!neighbors) {
        *count = 0;
        return NULL;
    }
    
    // Remplir le tableau
    current = graph->adjacencyList[nodeIndex]->next;
    int i = 0;
    while (current && i < *count) {
        // Vérifier que l'index est valide avant d'accéder à l'adjacencyList
        if (current->x >= 0 && current->x < graph->node_count && graph->adjacencyList[current->x]) {
            neighbors[i] = graph->adjacencyList[current->x];
            i++;
        }
        current = current->next;
    }
    
    // Ajuster le count si certains voisins étaient invalides
    *count = i;
    
    return neighbors;
}

int getNeighborCount(Graph* graph, int nodeIndex) {
    if (!graph || nodeIndex < 0 || nodeIndex >= graph->node_count) {
        return 0;
    }
    
    if (!graph->adjacencyList[nodeIndex]) {
        return 0;
    }
    
    int count = 0;
    Node* current = graph->adjacencyList[nodeIndex]->next;
    while (current) {
        count++;
        current = current->next;
    }
    
    return count;
}

bool areNeighbors(Graph* graph, int nodeA, int nodeB) {
    return hasEdge(graph, nodeA, nodeB);
}
