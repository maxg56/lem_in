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
            node->antCount = 0; // Ajout pour suivre le nombre de fourmis dans chaque nœud
        }
    }
}

Path *find_path(Graph *graph, bool *used_nodes) {
    if (!graph) return NULL;

    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);
    if (!startNode || !endNode) return NULL;

    int start = findNodeByName(graph, startNode->Nan);
    int end = findNodeByName(graph, endNode->Nan);
    if (start == -1 || end == -1) return NULL;

    int queue[graph->node_count];
    int front = 0, rear = 0;
    queue[rear++] = start;
    startNode->visited = true;
    startNode->parentNode = -1;

    while (front < rear) {
        int current_index = queue[front++];
        if (current_index == end) break;

        int count = 0;
        Node **neighbors = getNeighbors(graph, current_index, &count);
        for (int i = 0; i < count; i++) {
            int ni = findNodeByName(graph, neighbors[i]->Nan);
            if (ni != -1 && !neighbors[i]->visited) {
                bool can_use = true;
                if (!neighbors[i]->isStart && !neighbors[i]->isEnd && used_nodes[ni]) {
                    can_use = false;
                }

                if (can_use) {
                    neighbors[i]->visited = true;
                    neighbors[i]->parentNode = current_index;
                    queue[rear++] = ni;
                }
            }
        }
    }

    // if (!endNode->visited) {
    //     printf("Aucun chemin trouvé entre le départ et la fin.\n");
    //     return NULL;
    // }

    return build_path(graph, end);
}

void moveAnts(Graph *graph, Path **paths, int pathCount)
{
    Node *startNode = getStartNode(graph);
    Node *endNode = getEndNode(graph);

    if (!startNode || !endNode)
        return;

    // Déplacer les fourmis le long des chemins
    for (int i = 0; i < pathCount; i++)
    {
        Path *path = paths[i];
        for (int j = path->len - 1; j > 0; j--) // Parcourir le chemin à l'envers
        {
            Node *currentNode = getNodeByIndex(graph, path->nodes[j]);
            Node *previousNode = getNodeByIndex(graph, path->nodes[j - 1]);

            if (previousNode->antCount > 0) // Si une fourmi est présente dans le nœud précédent
            {
                currentNode->antCount++; // Ajouter une fourmi au nœud actuel
                previousNode->antCount--; // Retirer une fourmi du nœud précédent

                // Si la fourmi atteint le nœud final, elle est retirée du graphe
                if (currentNode == endNode)
                    currentNode->antCount--;
            }
        }
    }

    // Ajouter une nouvelle fourmi au départ si possible
    if (startNode->antCount > 0)
    {
        for (int i = 0; i < pathCount; i++)
        {
            Path *path = paths[i];
            Node *nextNode = getNodeByIndex(graph, path->nodes[1]);
            if (nextNode->antCount == 0) // Vérifier si le prochain nœud est libre
            {
                nextNode->antCount++;
                startNode->antCount--;
                break; // Une seule fourmi quitte le départ par tour
            }
        }
    }
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
        
        Path *path = find_path(graph, used_nodes);
        if (!path)
            break;
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
                    break;
                }
            }
        }
        
        if (is_duplicate)
            break;
        for (int j = 1; j < path->len - 1; j++)
            used_nodes[path->nodes[j]] = true;

        paths[pathCount++] = path;
        
        resetNodePositions(graph);
        attempt++;
    }
    
    *count = pathCount;
    return paths;
}