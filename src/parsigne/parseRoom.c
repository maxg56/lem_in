#include "lem-in.h"

bool parseRoom(Graph* graph, char* line, bool isStart, bool isEnd) {
    char **tokens = ft_arn_split(line, ' ');
    if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
        return false;
    char *name = ft_arn_strdup(tokens[0]);
    int x = ft_atoi(tokens[1]);
    int y = ft_atoi(tokens[2]);
    
    // Vérifier que les coordonnées sont valides
    if (!ft_isdigit(tokens[1][0]) && tokens[1][0] != '-') {
        return false;
    }
    if (!ft_isdigit(tokens[2][0]) && tokens[2][0] != '-') {
        return false;
    }
    
    
    addNode(graph, name, x, y, isStart, isEnd);

    return true;
}
