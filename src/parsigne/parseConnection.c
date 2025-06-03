#include "lem-in.h"

// Fonction helper pour parser une connexion
bool parseConnection(Graph* graph, char* line) {
    // Vérifier le format du lien d'abord
    if (!is_valid_link_format(line)) {
        set_parsing_error(ERROR_LINK_FORMAT, "Format de lien invalide");
        return false;
    }
    
    char **tokens = ft_arn_split(line, '-');
    if (!tokens || !tokens[0] || !tokens[1]) {
        set_parsing_error(ERROR_LINK_FORMAT, "Format de lien invalide - impossibile de séparer");
        return false;
    }
    
    // Vérifier qu'il n'y a que 2 tokens (pas de format a-b-c)
    if (tokens[2] != NULL) {
        set_parsing_error(ERROR_LINK_FORMAT, "Format de lien invalide (a-b-c)");
        return false;
    }
    
    // Trim newline characters from tokens
    char *name1 = ft_strtrim(tokens[0], " \t\n\r");
    char *name2 = ft_strtrim(tokens[1], " \t\n\r");
    
    if (!name1 || !name2 || !name1[0] || !name2[0]) {
        set_parsing_error(ERROR_LINK_FORMAT, "Noms de salles vides dans le lien");
        return false;
    }
    
    // Vérifier qu'il ne s'agit pas d'un self-link
    if (ft_strcmp(name1, name2) == 0) {
        set_parsing_error(ERROR_SELF_LINK, "Lien d'une salle vers elle-même");
        return false;
    }
    
    int idx1 = findNodeByName(graph, name1);
    int idx2 = findNodeByName(graph, name2);
    
    // Vérifier que les deux salles existent
    if (idx1 == -1) {
        set_parsing_error(ERROR_LINK_UNKNOWN_ROOM, ft_arn_strjoin("Lien avec salle non déclarée: ", name1));
        return false;
    }
    
    if (idx2 == -1) {
        set_parsing_error(ERROR_LINK_UNKNOWN_ROOM, ft_arn_strjoin("Lien avec salle non déclarée: ", name2));
        return false;
    }
    
    // Ajouter le lien
    addEdgeByName(graph, name1, name2);
    
    return true;
}