#include "lem-in.h"

Path *build_path(Graph *graph, int end_index)
{
    int tmp[graph->size];
    int len = 0;
    int current_index = end_index;

    while (current_index != -1) {
        tmp[len++] = current_index;
        current_index = getNodeByIndex(graph, current_index)->parentNode;
    }
    Path *path = ft_arnalloc(sizeof(Path));
    if(!path)
        return NULL;
    path->nodes = ft_arnalloc(sizeof(int) * len);
    if(!path->nodes) {
        free(path);
        return NULL;
    }
    for (int i = 0; i < len; i++)
        path->nodes[i] = tmp[len - 1 - i];
    path->len = len;
    path->assigned_ants = 0;
    return path;
}
