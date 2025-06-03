#include "lem-in.h"

Graph* parse_from_file(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    
    Graph* result = parsigense(fd);
    close(fd);
    return result;
}