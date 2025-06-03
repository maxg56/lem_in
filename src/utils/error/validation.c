#include "lem-in.h"

bool is_valid_room_name(const char *name) {
    if (!name || !name[0]) {
        return false;
    }
    
    // Le nom ne doit pas commencer par 'L' ou '#'
    if (name[0] == 'L' || name[0] == '#') {
        return false;
    }
    
    return true;
}

bool is_valid_coordinate(const char *coord_str) {
    if (!coord_str || !coord_str[0]) {
        return false;
    }
    
    int i = 0;
    
    // Permettre un signe négatif
    if (coord_str[i] == '-' || coord_str[i] == '+') {
        i++;
    }
    
    // Vérifier qu'il y a au moins un chiffre après le signe
    if (!coord_str[i]) {
        return false;
    }
    
    // Vérifier que tous les caractères restants sont des chiffres
    while (coord_str[i]) {
        if (!ft_isdigit(coord_str[i])) {
            return false;
        }
        i++;
    }
    
    return true;
}

bool has_duplicate_room(Graph* graph, const char *name) {
    if (!graph || !name) {
        return false;
    }
    
    for (int i = 0; i < graph->node_count; i++) {
        Node *node = getNodeByIndex(graph, i);
        if (node && node->Nan && ft_strcmp(node->Nan, name) == 0) {
            return true;
        }
    }
    
    return false;
}

bool has_same_coordinates(Graph* graph, int x, int y) {
    if (!graph) {
        return false;
    }
    
    for (int i = 0; i < graph->node_count; i++) {
        Node *node = getNodeByIndex(graph, i);
        if (node && node->x == x && node->y == y) {
            return true;
        }
    }
    
    return false;
}

bool is_valid_link_format(const char *line) {
    if (!line || !line[0]) {
        return false;
    }
    
    // Doit contenir exactement un tiret
    int dash_count = 0;
    int i = 0;
    
    while (line[i]) {
        if (line[i] == '-') {
            dash_count++;
        }
        i++;
    }
    
    if (dash_count != 1) {
        return false;
    }
    
    // Vérifier qu'il n'y a pas d'espaces autour du tiret
    char *dash_pos = ft_strchr(line, '-');
    if (!dash_pos) {
        return false;
    }
    
    // Vérifier que ce n'est pas au début ou à la fin
    if (dash_pos == line || *(dash_pos + 1) == '\0' || *(dash_pos + 1) == '\n') {
        return false;
    }
    
    return true;
}

bool parsing_phase_allows_links(Graph* graph) {
    // Pour l'instant, on autorise toujours les liens
    // Cette fonction peut être étendue pour vérifier si toutes les salles ont été définies
    (void)graph; // Supprime le warning du paramètre non utilisé
    return true;
}

// Fonction pour valider une ligne vide (autorisée seulement en fin de fichier pour les commentaires)
bool is_empty_line_valid(const char *line, bool end_of_file) {
    if (!line) {
        return false;
    }
    
    // Ligne vide ou contenant seulement des espaces
    int i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r')) {
        i++;
    }
    
    if (line[i] == '\0') {
        // C'est une ligne vide - seulement autorisée en fin de fichier
        return end_of_file;
    }
    
    return true; // Ce n'est pas une ligne vide
}
