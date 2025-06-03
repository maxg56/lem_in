#include "data.h"

void addEdge(Graph* graph, int nodeA, int nodeB) {
    if (!graph || nodeA < 0 || nodeB < 0 || nodeA >= graph->size || nodeB >= graph->size) {
        return;
    }
    
    if (!graph->adjacencyList[nodeA] || !graph->adjacencyList[nodeB]) {
        return;
    }
    
    // Créer une nouvelle connexion pour nodeA vers nodeB
    Node* newConnectionA = (Node*)ft_arnalloc(sizeof(Node));
    if (!newConnectionA) return;
    
    newConnectionA->Nan = graph->adjacencyList[nodeB]->Nan;
    newConnectionA->x = nodeB; // Utilise x pour stocker l'index
    newConnectionA->y = graph->adjacencyList[nodeB]->y;
    newConnectionA->isStart = false;
    newConnectionA->isEnd = false;
    newConnectionA->next = graph->adjacencyList[nodeA]->next;
    graph->adjacencyList[nodeA]->next = newConnectionA;
    
    // Créer une nouvelle connexion pour nodeB vers nodeA (graphe non-orienté)
    Node* newConnectionB = (Node*)ft_arnalloc(sizeof(Node));
    if (!newConnectionB) return;
    
    newConnectionB->Nan = graph->adjacencyList[nodeA]->Nan;
    newConnectionB->x = nodeA; // Utilise x pour stocker l'index
    newConnectionB->y = graph->adjacencyList[nodeA]->y;
    newConnectionB->isStart = false;
    newConnectionB->isEnd = false;
    newConnectionB->next = graph->adjacencyList[nodeB]->next;
    graph->adjacencyList[nodeB]->next = newConnectionB;
}

void addEdgeByName(Graph* graph, char* nameA, char* nameB) {
    if (!graph || !nameA || !nameB) return;
    
    int indexA = findNodeByName(graph, nameA);
    int indexB = findNodeByName(graph, nameB);
    
    if (indexA != -1 && indexB != -1) {
        addEdge(graph, indexA, indexB);
    }
}

bool hasEdge(Graph* graph, int nodeA, int nodeB) {
    if (!graph || nodeA < 0 || nodeB < 0 || nodeA >= graph->size || nodeB >= graph->size) {
        return false;
    }
    
    if (!graph->adjacencyList[nodeA]) {
        return false;
    }
    
    Node* current = graph->adjacencyList[nodeA]->next;
    while (current) {
        if (current->x == nodeB) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

void removeEdge(Graph* graph, int nodeA, int nodeB) {
    if (!graph || nodeA < 0 || nodeB < 0 || nodeA >= graph->size || nodeB >= graph->size) {
        return;
    }
    
    // Supprimer la connexion de A vers B
    if (graph->adjacencyList[nodeA] && graph->adjacencyList[nodeA]->next) {
        Node* current = graph->adjacencyList[nodeA];
        while (current->next) {
            if (current->next->x == nodeB) {
                Node* toDelete = current->next;
                current->next = toDelete->next;
                // Note: pas de free avec arena allocator
                break;
            }
            current = current->next;
        }
    }
    
    // Supprimer la connexion de B vers A
    if (graph->adjacencyList[nodeB] && graph->adjacencyList[nodeB]->next) {
        Node* current = graph->adjacencyList[nodeB];
        while (current->next) {
            if (current->next->x == nodeA) {
                Node* toDelete = current->next;
                current->next = toDelete->next;
                // Note: pas de free avec arena allocator
                break;
            }
            current = current->next;
        }
    }
}
