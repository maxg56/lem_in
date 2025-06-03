#include "lem-in.h"

Path *build_path(Graph *graph, int end_index)
{
    if (!graph || end_index < 0 || end_index >= graph->node_count) {
        return NULL;
    }
    
    int tmp[graph->node_count];
    int len = 0;
    int current_index = end_index;
    int max_iterations = graph->node_count; // Prevent infinite loops

    while (current_index != -1 && len < max_iterations) {
        tmp[len++] = current_index;
        Node* current_node = getNodeByIndex(graph, current_index);
        if (!current_node) break;
        current_index = current_node->parentNode;
    }
    
    if (len == 0) return NULL;
    
    Path *path = ft_arnalloc(sizeof(Path));
    if(!path)
        return NULL;
    path->nodes = ft_arnalloc(sizeof(int) * len);
    if(!path->nodes) {
        return NULL; // With arena allocator, no need to free
    }
    for (int i = 0; i < len; i++)
        path->nodes[i] = tmp[len - 1 - i];
    path->len = len;
    path->assigned_ants = 0;
    return path;
}
