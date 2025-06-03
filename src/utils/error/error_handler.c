#include "lem-in.h"
#include <stdio.h>

// Global error state
static ParsingError g_parsing_error = ERROR_NONE;
static char *g_error_message = NULL;

void set_parsing_error(ParsingError error, const char *custom_message) {
    g_parsing_error = error;
    if (g_error_message) {
        // Libérer le message précédent si nécessaire
        g_error_message = NULL;
    }
    
    if (custom_message) {
        g_error_message = ft_arn_strdup((char *)custom_message);
    }
}

ParsingError get_parsing_error(void) {
    return g_parsing_error;
}

const char* get_error_message(void) {
    static const char* error_messages[] = {
        [ERROR_NONE] = "No error",
        [ERROR_EMPTY_LINE] = " Ligne vide (hors commentaires en fin de fichier)",
        [ERROR_UNKNOWN_COMMAND] = " Ligne avec une commande inconnue non ignorée correctement",
        [ERROR_INVALID_FORMAT] = " Format non reconnu (ni salle, ni lien, ni commentaire)",
        [ERROR_EMPTY_ANT_LINE] = " Ligne vide pour le nombre de fourmis",
        [ERROR_NON_NUMERIC_ANTS] = " Nombre de fourmis non numérique",
        [ERROR_INVALID_ANT_COUNT] = " Nombre de fourmis ≤ 0",
        [ERROR_ROOM_NAME_INVALID] = " Nom de salle commence par L ou #",
        [ERROR_ROOM_FORMAT] = " Salle mal formatée (ex: room 2)",
        [ERROR_INVALID_COORDINATES] = " Coordonnées non valides (non entiers)",
        [ERROR_DUPLICATE_ROOM] = " Doublon de noms de salle",
        [ERROR_SAME_COORDINATES] = "🔸 Coordonnées identiques pour deux salles",
        [ERROR_LINK_FORMAT] = " Format de lien invalide (a-b-c, a - b, etc.)",
        [ERROR_LINK_UNKNOWN_ROOM] = " Lien avec salle non déclarée",
        [ERROR_SELF_LINK] = " Lien d'une salle vers elle-même (room-room)",
        [ERROR_COMMAND_NOT_FOLLOWED] = " ##start ou ##end non suivis d'une vraie salle",
        [ERROR_MULTIPLE_COMMANDS] = " Plusieurs ##start ou ##end",
        [ERROR_NO_START_END] = " Aucun ##start ou ##end",
        [ERROR_LINKS_BEFORE_ROOMS] = " Déclaration des liens avant que toutes les salles soient définies",
        [ERROR_INSUFFICIENT_DATA] = " Moins de deux salles",
        [ERROR_NO_LINKS] = " Pas de liens",
        [ERROR_NO_PATH] = " Pas de chemin possible entre start et end (graphe déconnecté)"
    };
    
    if (g_error_message) {
        return g_error_message;
    }
    
    if (g_parsing_error >= 0 && g_parsing_error < sizeof(error_messages) / sizeof(error_messages[0])) {
        return error_messages[g_parsing_error];
    }
    
    return "Erreur inconnue";
}

void print_parsing_error(void) {
    if (g_parsing_error != ERROR_NONE) {
        printf("Error: %s\n", get_error_message());
    }
}

void clear_parsing_error(void) {
    g_parsing_error = ERROR_NONE;
    g_error_message = NULL;
}

bool has_parsing_error(void) {
    return g_parsing_error != ERROR_NONE;
}
