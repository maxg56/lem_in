#ifndef LEM_IN_H
# define LEM_IN_H

# include "data.h"
# include "libft.h"
#include <fcntl.h>  // Pour open()
#include <unistd.h> // Pour close()

Graph* parsigense(int fd);
Graph* parse_from_file(const char* filename);
bool parseRoom(Graph* graph, char* line, bool isStart, bool isEnd);
bool parseConnection(Graph* graph, char* line);

// Gestion des flags
void init_flags(void);
bool parse_flags(int argc, char *argv[], char **filename);
void print_usage(const char *program_name);

// Getters pour les flags
bool is_verbose(void);
bool is_debug(void);
bool show_paths(void);
bool show_stats(void);
bool is_help(void);
bool is_quiet(void);
bool is_benchmark(void);
bool is_visual(void);

// Fonctions d'affichage conditionnelles
void debug_printf(const char *format, ...);
void verbose_printf(const char *format, ...);

#endif