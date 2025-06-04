#ifndef DATA_H
# define DATA_H

#include <stdbool.h>
#include <stdlib.h>
#  include "libft.h"

// Énumération des erreurs de parsing
typedef enum {
    ERROR_NONE = 0,
    // Format de fichier
    ERROR_EMPTY_LINE,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_FORMAT,
    // Nombre de fourmis
    ERROR_EMPTY_ANT_LINE,
    ERROR_NON_NUMERIC_ANTS,
    ERROR_INVALID_ANT_COUNT,
    // Définition de salle
    ERROR_ROOM_NAME_INVALID,
    ERROR_ROOM_FORMAT,
    ERROR_INVALID_COORDINATES,
    ERROR_DUPLICATE_ROOM,
    ERROR_SAME_COORDINATES, // Facultatif
    // Définition de lien
    ERROR_LINK_FORMAT,
    ERROR_LINK_UNKNOWN_ROOM,
    ERROR_SELF_LINK,
    // Commandes spéciales
    ERROR_COMMAND_NOT_FOLLOWED,
    ERROR_MULTIPLE_COMMANDS,
    // Ordre et logique
    ERROR_NO_START_END,
    ERROR_LINKS_BEFORE_ROOMS,
    // Données insuffisantes
    ERROR_INSUFFICIENT_DATA,
    ERROR_NO_LINKS,
    ERROR_NO_PATH
} ParsingError;


typedef struct Node {
    char *Nan;
    bool isStart;
    bool isEnd;
    bool visited; 
    int x;
    int y;
    int parentNode;
    struct Node* next;
} Node;

typedef struct Graph {
    int size;           // Capacity (for compatibility with tests)
    int node_count;     // Number of actually used nodes
    int size_max;       // Maximum capacity for dynamic resizing
    int nb_fourmis;
    Node** adjacencyList;
} Graph;

typedef struct Path{
    int *nodes;
    int len;
    int assigned_ants; 
} Path;

typedef struct Ants{
    int id;
    int position; // Index of the current node in the path
    int path_index; // Index of the path in the graph
    bool arrived; // Indicates if the ant has reached the end
} Ants;

Graph* createGraph(int size);
void displayGraph(Graph* graph);
void addNode(Graph* graph, char * Nan, int x , int y, bool isStart, bool isEnd);
void resetNodePosition(void);

// Gestion des connexions/arêtes
void addEdge(Graph* graph, int nodeA, int nodeB);
void addEdgeByName(Graph* graph, char* nameA, char* nameB);
bool hasEdge(Graph* graph, int nodeA, int nodeB);
void removeEdge(Graph* graph, int nodeA, int nodeB);

// Recherche et navigation
int findNodeByName(Graph* graph, char* name);
Node* getNodeByIndex(Graph* graph, int index);
Node* getStartNode(Graph* graph);
Node* getEndNode(Graph* graph);
int getNodeCount(Graph* graph);

// Fonctions d'adjacence
Node** getNeighbors(Graph* graph, int nodeIndex, int* count);
int getNeighborCount(Graph* graph, int nodeIndex);
bool areNeighbors(Graph* graph, int nodeA, int nodeB);

// Validation
bool validateGraph(Graph* graph);
bool hasStartEnd(Graph* graph);
bool isConnected(Graph* graph, int nodeA, int nodeB);

// Gestion d'erreurs
void set_parsing_error(ParsingError error, const char *custom_message);
ParsingError get_parsing_error(void);
const char* get_error_message(void);
void print_parsing_error(void);
void clear_parsing_error(void);
bool has_parsing_error(void);

// Fonctions de validation avancées
bool is_valid_room_name(const char *name);
bool is_valid_coordinate(const char *coord_str);
bool has_duplicate_room(Graph* graph, const char *name);
bool has_same_coordinates(Graph* graph, int x, int y);
bool is_valid_link_format(const char *line);
bool parsing_phase_allows_links(Graph* graph);
bool is_empty_line_valid(const char *line, bool end_of_file);

// Resize functions
bool resizeGraph(Graph* graph, int newSize);

//Algorithmes
Path* find_path(Graph* graph);
Path **findAllPaths(Graph* graph, int *count);
Path *build_path(Graph *graph, int end_index);

//Mouvements des fourmis
void antsMovements(Graph *graph, Path *path);
void multiplePaths(Graph *graph, Path **paths, int pathCount);
void assignAnts(Path **paths, int path_count, int total_ants);

#endif
