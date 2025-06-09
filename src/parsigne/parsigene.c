#include "lem-in.h"

Graph* parsigense(int fd) {
    Graph *graph = NULL;
    char *line;
    bool isStart = false;
    bool isEnd = false;
    bool hasStart = false;
    bool hasEnd = false;
    bool expecting_start_room = false;
    bool expecting_end_room = false;
    int initialSize = 10;
    int line_count = 0;
    bool first_ant_line_processed = false;

    // Clear any previous parsing errors
    clear_parsing_error();

    if (fd < 0) {
        set_parsing_error(ERROR_INVALID_FORMAT, "Fichier invalide");
        return NULL;
    }
    
    graph = createGraph(initialSize);
    if (!graph) {
        set_parsing_error(ERROR_INVALID_FORMAT, "Échec de création du graphe");
        return NULL;
    }

    while ((line = arn_get_next_line(fd))) {
        line_count++;
        
        // Vérifier les lignes vides (sauf pour les commentaires en fin de fichier)
        if (!line || line[0] == '\0' || (line[0] == '\n' && line[1] == '\0')) {
            // Pour l'instant, on ignore les lignes vides mais on pourrait les vérifier en fin de fichier
            continue;
        }
        
        // Trim whitespace
        char *trimmed_line = ft_arn_strtrim(line, " \t\n\r");
        if (!trimmed_line || trimmed_line[0] == '\0') {
            continue;
        }
        
        // Gestion des commandes ##start et ##end
        if (ft_strncmp(trimmed_line, "##start", 7) == 0) {
            // Vérifier si on attendait une salle après ##end
            if (expecting_end_room) {
                set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
                return NULL;
            }
            if (hasStart) {
                set_parsing_error(ERROR_MULTIPLE_COMMANDS, "Plusieurs ##start trouvés");
                return NULL;
            }
            hasStart = true;
            isStart = true;
            expecting_start_room = true;
            continue;
        }
        
        if (ft_strncmp(trimmed_line, "##end", 5) == 0) {
            // Vérifier si on attendait une salle après ##start
            if (expecting_start_room) {
                set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
                return NULL;
            }
            if (hasEnd) {
                set_parsing_error(ERROR_MULTIPLE_COMMANDS, "Plusieurs ##end trouvés");
                return NULL;
            }
            hasEnd = true;
            isEnd = true;
            expecting_end_room = true;
            continue;
        }
        
        // Ignorer les autres commentaires
        if (trimmed_line[0] == '#') {
            // Vérifier si on attendait une salle après ##start ou ##end
            if (expecting_start_room || expecting_end_room) {
                set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
                return NULL;
            }
            continue;
        }
        
        // Première ligne : nombre de fourmis
        if (graph->nb_fourmis == 0 && !first_ant_line_processed) {
            first_ant_line_processed = true;
            
            // Vérifier ligne vide
            if (!trimmed_line[0]) {
                set_parsing_error(ERROR_EMPTY_ANT_LINE, "Ligne vide pour le nombre de fourmis");
                return NULL;
            }
            
            // Validate that the line contains only digits
            int i = 0;
            
            // Skip leading whitespace (déjà trimé)
            while (trimmed_line[i] && (trimmed_line[i] == ' ' || trimmed_line[i] == '\t')) {
                i++;
            }
            
            // Check if empty after trimming whitespace
            if (!trimmed_line[i]) {
                set_parsing_error(ERROR_EMPTY_ANT_LINE, "Ligne vide pour le nombre de fourmis");
                return NULL;
            }
            
            // Check each character is a digit
            while (trimmed_line[i] && trimmed_line[i] != '\n' && trimmed_line[i] != '\r') {
                if (!ft_isdigit(trimmed_line[i])) {
                    set_parsing_error(ERROR_NON_NUMERIC_ANTS, "Nombre de fourmis non numérique");
                    return NULL;
                }
                i++;
            }
            
            int ant_count = ft_atoi(trimmed_line);
            if (ant_count <= 0) {
                set_parsing_error(ERROR_INVALID_ANT_COUNT, "Nombre de fourmis ≤ 0");
                return NULL;
            }
            
            graph->nb_fourmis = ant_count;
            continue;
        }
        
        // Ligne de connexion (contient un tiret)
        if (ft_strchr(trimmed_line, '-')) {
            // Vérifier si on attend une salle après ##start ou ##end
            if (expecting_start_room || expecting_end_room) {
                set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
                return NULL;
            }
            
            // Vérifier le format du lien
            if (!is_valid_link_format(trimmed_line)) {
                set_parsing_error(ERROR_LINK_FORMAT, "Format de lien invalide");
                return NULL;
            }
            
            // Vérifier que les liens ne sont pas déclarés avant les salles
            if (!parsing_phase_allows_links(graph)) {
                set_parsing_error(ERROR_LINKS_BEFORE_ROOMS, "Déclaration des liens avant que toutes les salles soient définies");
                return NULL;
            }
            
            if (!parseConnection(graph, trimmed_line)) {
                return NULL; // L'erreur est déjà définie dans parseConnection
            }
            continue;
        }
        
        // Ligne de salle : nom x y
        bool room_parsed = parseRoom(graph, trimmed_line, isStart, isEnd);
        
        if (room_parsed) {
            // Reset des flags après traitement réussi d'une salle
            if (isStart) {
                expecting_start_room = false;
            }
            if (isEnd) {
                expecting_end_room = false;
            }
            isStart = false;
            isEnd = false;
        } else {
            // Si ce n'est ni une salle valide ni un lien, vérifier si on attendait une salle
            if (expecting_start_room || expecting_end_room) {
                set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
                return NULL;
            }
            return NULL; // L'erreur est déjà définie dans parseRoom
        }
    }

    // Vérifier si on attendait encore une salle après ##start ou ##end à la fin du fichier
    if (expecting_start_room || expecting_end_room) {
        set_parsing_error(ERROR_COMMAND_NOT_FOLLOWED, "##start ou ##end non suivis d'une vraie salle");
        return NULL;
    }

    // Validation finale du graphe
    
    // Vérifier qu'on a au moins start et end
    if (!hasStart || !hasEnd) {
        set_parsing_error(ERROR_NO_START_END, "Aucun ##start ou ##end");
        return NULL;
    }
    
    // Vérifier qu'on a au moins 2 salles
    if (graph->node_count < 2) {
        set_parsing_error(ERROR_INSUFFICIENT_DATA, "Moins de deux salles");
        return NULL;
    }
    
    // Vérifier qu'on a des liens
    bool has_links = false;
    for (int i = 0; i < graph->node_count && !has_links; i++) {
        if (getNeighborCount(graph, i) > 0) {
            has_links = true;
        }
    }
    
    if (!has_links) {
        set_parsing_error(ERROR_NO_LINKS, "Pas de liens");
        return NULL;
    }
    
    // Vérifier qu'il y a un chemin entre start et end
    // if (!validateGraph(graph)) {
    //     set_parsing_error(ERROR_NO_PATH, "Pas de chemin possible entre start et end");
    //     return NULL;
    // }

    // Ajuster la taille finale du graphe au nombre réel de nœuds
    if (graph->node_count > 0 && graph->node_count < graph->size_max) {
        // Optionnel: réduire la taille pour économiser la mémoire
        // Pour l'instant, on garde la taille actuelle
    }

    return graph;
}
