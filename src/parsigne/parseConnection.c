#include "lem-in.h"
// Fonction helper pour parser une connexion
bool parseConnection(Graph* graph, char* line) {
    char **tokens = ft_arn_split(line, '-');
    if (!tokens || !tokens[0] || !tokens[1]) {
        return false;
    }
    
    // Trim newline characters from tokens
    char *name1 = ft_strtrim(tokens[0], " \t\n\r");
    char *name2 = ft_strtrim(tokens[1], " \t\n\r");
    
    if (!name1 || !name2) {
        return false;
    }
    
    int idx1 = findNodeByName(graph, name1);
    int idx2 = findNodeByName(graph, name2);
    
    if (idx1 != -1 && idx2 != -1) {
        addEdgeByName(graph, name1, name2);
    }
    
    return true;
}