#include "data.h"

bool validateGraph(Graph* graph) {
    if (!graph || !graph->adjacencyList) {
        return false;
    }
    
    // Vérifier qu'il y a au moins une salle de départ et une d'arrivée
    if (!hasStartEnd(graph)) {
        return false;
    }
    
    // Vérifier qu'il y a au moins 1 nœud
    if (getNodeCount(graph) < 1) {
        return false;
    }
    
    // Vérifier que le graphe est connecté (start peut atteindre end)
    Node* start = getStartNode(graph);
    Node* end = getEndNode(graph);
    if (!start || !end) {
        return false;
    }
    
    int startIndex = findNodeByName(graph, start->Nan);
    int endIndex = findNodeByName(graph, end->Nan);
    
    // Si c'est la même salle (start == end), c'est valide
    if (startIndex == endIndex) {
        return true;
    }
    
    return isConnected(graph, startIndex, endIndex);
}

bool hasStartEnd(Graph* graph) {
    if (!graph) return false;
    
    bool hasStart = false;
    bool hasEnd = false;
    
    for (int i = 0; i < graph->size; i++) {
        if (graph->adjacencyList[i]) {
            if (graph->adjacencyList[i]->isStart) {
                if (hasStart) {
                    // Plus d'un nœud de départ
                    return false;
                }
                hasStart = true;
            }
            if (graph->adjacencyList[i]->isEnd) {
                if (hasEnd) {
                    // Plus d'un nœud d'arrivée
                    return false;
                }
                hasEnd = true;
            }
        }
    }
    
    return hasStart && hasEnd;
}

bool isGraphConnected(Graph* graph) {
    if (!graph) return false;
    
    int nodeCount = getNodeCount(graph);
    if (nodeCount <= 1) return true;
    
    // Utiliser BFS pour vérifier la connectivité depuis le premier nœud non-NULL
    int startIndex = -1;
    for (int i = 0; i < graph->size; i++) {
        if (graph->adjacencyList[i]) {
            startIndex = i;
            break;
        }
    }
    
    if (startIndex == -1) return false;
    
    // Tableau pour marquer les nœuds visités
    bool* visited = (bool*)ft_arn_calloc(graph->size, sizeof(bool));
    if (!visited) return false;
    
    // Queue simple pour BFS
    int* queue = (int*)ft_arnalloc(sizeof(int) * graph->size);
    if (!queue) return false;
    
    int front = 0, rear = 0;
    queue[rear++] = startIndex;
    visited[startIndex] = true;
    int visitedCount = 1;
    
    while (front < rear) {
        int current = queue[front++];
        
        // Parcourir tous les voisins
        Node* neighbor = graph->adjacencyList[current]->next;
        while (neighbor) {
            int neighborIndex = neighbor->x;
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = true;
                queue[rear++] = neighborIndex;
                visitedCount++;
            }
            neighbor = neighbor->next;
        }
    }
    
    return visitedCount == nodeCount;
}

bool isConnected(Graph* graph, int nodeA, int nodeB) {
    if (!graph || nodeA < 0 || nodeB < 0 || nodeA >= graph->size || nodeB >= graph->size) {
        return false;
    }
    
    if (nodeA == nodeB) return true;
    
    if (!graph->adjacencyList[nodeA] || !graph->adjacencyList[nodeB]) {
        return false;
    }
    
    // Utiliser BFS pour trouver un chemin de A vers B
    bool* visited = (bool*)ft_arn_calloc(graph->size, sizeof(bool));
    if (!visited) return false;
    
    int* queue = (int*)ft_arnalloc(sizeof(int) * graph->size);
    if (!queue) return false;
    
    int front = 0, rear = 0;
    queue[rear++] = nodeA;
    visited[nodeA] = true;
    
    while (front < rear) {
        int current = queue[front++];
        
        if (current == nodeB) {
            return true;
        }
        
        // Parcourir tous les voisins
        Node* neighbor = graph->adjacencyList[current]->next;
        while (neighbor) {
            int neighborIndex = neighbor->x;
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = true;
                queue[rear++] = neighborIndex;
            }
            neighbor = neighbor->next;
        }
    }
    
    return false;
}
