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


#endif