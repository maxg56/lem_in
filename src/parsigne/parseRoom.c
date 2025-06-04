#include "lem-in.h"

bool parseRoom(Graph* graph, char* line, bool isStart, bool isEnd) {
    char **tokens = ft_arn_split(line, ' ');
    
    // Vérifier le format de base (nom x y)
    if (!tokens || !tokens[0] || !tokens[1] || !tokens[2]) {
        set_parsing_error(ERROR_ROOM_FORMAT, "Salle mal formatée - format attendu: nom x y");
        return false;
    }
    
    // Vérifier qu'il n'y a pas plus de 3 tokens
    if (tokens[3] != NULL) {
        set_parsing_error(ERROR_ROOM_FORMAT, "Salle mal formatée - trop d'arguments");
        return false;
    }
    
    char *name = ft_arn_strtrim(tokens[0], " \t\n\r");
    char *x_str = ft_arn_strtrim(tokens[1], " \t\n\r");
    char *y_str = ft_arn_strtrim(tokens[2], " \t\n\r");
    
    if (!name || !x_str || !y_str) {
        set_parsing_error(ERROR_ROOM_FORMAT, "Erreur lors du parsing des tokens");
        return false;
    }
    
    // Vérifier le nom de la salle
    if (!is_valid_room_name(name)) {
        set_parsing_error(ERROR_ROOM_NAME_INVALID, "Nom de salle commence par L ou #");
        return false;
    }
    
    // Vérifier si la salle existe déjà
    if (has_duplicate_room(graph, name)) {
        set_parsing_error(ERROR_DUPLICATE_ROOM, "Doublon de noms de salle");
        return false;
    }
    
    // Vérifier que les coordonnées sont valides
    if (!is_valid_coordinate(x_str)) {
        set_parsing_error(ERROR_INVALID_COORDINATES, "Coordonnée X non valide (non entier)");
        return false;
    }
    
    if (!is_valid_coordinate(y_str)) {
        set_parsing_error(ERROR_INVALID_COORDINATES, "Coordonnée Y non valide (non entier)");
        return false;
    }
    
    int x = ft_atoi(x_str);
    int y = ft_atoi(y_str);
    
    // Vérifier les coordonnées identiques (facultatif - warning seulement)
    if (has_same_coordinates(graph, x, y)) {
        set_parsing_error(ERROR_SAME_COORDINATES, "Coordonnées identiques pour deux salles");
        // Continue malgré cette erreur car c'est facultatif
    }
    
    addNode(graph, name, x, y, isStart, isEnd);

    return true;
}
