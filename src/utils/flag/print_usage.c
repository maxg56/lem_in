
#include "lem-in.h"

void print_usage(const char *program_name) {
    ft_putstr_fd("Usage: ", 1);
    ft_putstr_fd((char*)program_name, 1);
    ft_putstr_fd(" [OPTIONS] [FICHIER]\n\n", 1);
    
    ft_putstr_fd("DESCRIPTION:\n", 1);
    ft_putstr_fd("  Résout le problème lem-in en trouvant le chemin optimal pour faire\n", 1);
    ft_putstr_fd("  passer toutes les fourmis du début à la fin.\n\n", 1);
    
    ft_putstr_fd("OPTIONS:\n", 1);
    ft_putstr_fd("  -v, --verbose    Affichage détaillé du processus\n", 1);
    ft_putstr_fd("  -d, --debug      Mode debug avec informations techniques\n", 1);
    ft_putstr_fd("  -p, --paths      Afficher les chemins trouvés\n", 1);
    ft_putstr_fd("  -s, --stats      Afficher les statistiques (nombre de tours, etc.)\n", 1);
    ft_putstr_fd("  -q, --quiet      Mode silencieux (sortie minimale)\n", 1);
    ft_putstr_fd("  -b, --benchmark  Mesurer le temps d'exécution\n", 1);
    ft_putstr_fd("  --used-paths     Afficher seulement les chemins utilisés par les fourmis\n", 1);
    ft_putstr_fd("  --all-paths      Afficher tous les chemins trouvés par l'algorithme\n", 1);
    ft_putstr_fd("  -h, --help       Afficher cette aide\n\n", 1);
    
    ft_putstr_fd("EXEMPLES:\n", 1);
    ft_putstr_fd("  ", 1);
    ft_putstr_fd((char*)program_name, 1);
    ft_putstr_fd(" map.map                    # Résoudre avec map.map\n", 1);
    ft_putstr_fd("  ", 1);
    ft_putstr_fd((char*)program_name, 1);
    ft_putstr_fd(" -v -p map.map             # Mode verbose avec affichage des chemins\n", 1);
    ft_putstr_fd("  ", 1);
    ft_putstr_fd((char*)program_name, 1);
    ft_putstr_fd(" -s < map.map              # Statistiques en lisant depuis stdin\n", 1);
}