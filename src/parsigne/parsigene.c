#include "lem-in.h"

Graph* parsigense(int fd) {
    Graph *graph = NULL;
    char *line;
    bool isStart = false;
    bool isEnd = false;
    int initialSize = 10;

    if (fd < 0) {
        return NULL; // Fichier invalide
    }
    graph = createGraph(initialSize);
    if (!graph) {
        return NULL;
    }

    while ((line = arn_get_next_line(fd))) {
        if (!line || line[0] == '\0') {
            continue;
        }
        
        if (ft_strncmp(line, "##start", 7) == 0) {
            isStart = true;
            continue;
        }
        if (ft_strncmp(line, "##end", 5) == 0) {
            isEnd = true;
            continue;
        }
        
        // Ignorer les autres commentaires
        if (line[0] == '#') {
            continue;
        }
        
        // Première ligne : nombre de fourmis
        if (graph->nb_fourmis == 0) {
            // Validate that the line contains only digits
            bool valid_ant_count = true;
            int i = 0;
            
            // Skip leading whitespace
            while (line[i] && (line[i] == ' ' || line[i] == '\t')) {
                i++;
            }
            
            // Check if empty after trimming whitespace
            if (!line[i]) {
                valid_ant_count = false;
            } else {
                // Check each character is a digit
                while (line[i] && line[i] != '\n' && line[i] != '\r') {
                    if (!ft_isdigit(line[i])) {
                        valid_ant_count = false;
                        break;
                    }
                    i++;
                }
            }
            
            if (valid_ant_count) {
                int ant_count = ft_atoi(line);
                if (ant_count > 0) {
                    graph->nb_fourmis = ant_count;
                    continue;
                }
            }
            
            // Invalid ant count - return NULL to indicate parsing failure
            return NULL;
        }
        
        // Ligne de connexion (contient un tiret)
        if (ft_strchr(line, '-')) {
            parseConnection(graph, line);
            continue;
        }
        
        // Ligne de salle : nom x y
        if (parseRoom(graph, line, isStart, isEnd)) {
            isStart = false;
            isEnd = false;
        }
    }

    // Ajuster la taille finale du graphe au nombre réel de nœuds
    if (graph->node_count > 0 && graph->node_count < graph->size_max) {
        // Optionnel: réduire la taille pour économiser la mémoire
        // Pour l'instant, on garde la taille actuelle
    }

    return graph;
}
