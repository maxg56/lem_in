#include "lem-in.h"


// Structure pour gérer les flags
typedef struct {
    bool verbose;           // -v : affichage détaillé
    bool debug;            // -d : mode debug
    bool show_paths;       // -p : afficher les chemins trouvés
    bool show_stats;       // -s : afficher les statistiques
    bool help;             // -h : afficher l'aide
    bool quiet;            // -q : mode silencieux
    bool benchmark;        // -b : mode benchmark/timing
    bool show_used_paths;  // --used-paths : afficher seulement les chemins utilisés
    bool show_all_paths;   // --all-paths : afficher tous les chemins trouvés
} Flags;

static Flags g_flags = {0};

void init_flags(void) {
    g_flags.verbose = false;
    g_flags.debug = false;
    g_flags.show_paths = false;
    g_flags.show_stats = false;
    g_flags.help = false;
    g_flags.quiet = false;
    g_flags.benchmark = false;
    g_flags.show_used_paths = false;
    g_flags.show_all_paths = false;
}

bool parse_flags(int argc, char *argv[], char **filename) {
    int i = 1;
    *filename = NULL;
    
    while (i < argc) {
        if (argv[i][0] == '-') {
            // Flags courts
            if (ft_strcmp(argv[i], "-v") == 0 || ft_strcmp(argv[i], "--verbose") == 0) {
                g_flags.verbose = true;
            }
            else if (ft_strcmp(argv[i], "-d") == 0 || ft_strcmp(argv[i], "--debug") == 0) {
                g_flags.debug = true;
            }
            else if (ft_strcmp(argv[i], "-p") == 0 || ft_strcmp(argv[i], "--paths") == 0) {
                g_flags.show_paths = true;
            }
            else if (ft_strcmp(argv[i], "-s") == 0 || ft_strcmp(argv[i], "--stats") == 0) {
                g_flags.show_stats = true;
            }
            else if (ft_strcmp(argv[i], "-h") == 0 || ft_strcmp(argv[i], "--help") == 0) {
                g_flags.help = true;
            }
            else if (ft_strcmp(argv[i], "-q") == 0 || ft_strcmp(argv[i], "--quiet") == 0) {
                g_flags.quiet = true;
            }
            else if (ft_strcmp(argv[i], "-b") == 0 || ft_strcmp(argv[i], "--benchmark") == 0) {
                g_flags.benchmark = true;
            }
            else if (ft_strcmp(argv[i], "--used-paths") == 0) {
                g_flags.show_used_paths = true;
            }
            else if (ft_strcmp(argv[i], "--all-paths") == 0) {
                g_flags.show_all_paths = true;
            }
            else {
                ft_putstr_fd("Erreur: Flag inconnu '", 2);
                ft_putstr_fd(argv[i], 2);
                ft_putstr_fd("'\n", 2);
                return false;
            }
        }
        else {
            // Fichier d'entrée
            if (*filename == NULL) {
                *filename = argv[i];
            }
            else {
                ft_putstr_fd("Erreur: Plusieurs fichiers spécifiés\n", 2);
                return false;
            }
        }
        i++;
    }
    
    return true;
}



// Getters pour les flags
bool is_verbose(void) { return g_flags.verbose; }
bool is_debug(void) { return g_flags.debug; }
bool show_paths(void) { return g_flags.show_paths; }
bool show_stats(void) { return g_flags.show_stats; }
bool is_help(void) { return g_flags.help; }
bool is_quiet(void) { return g_flags.quiet; }
bool is_benchmark(void) { return g_flags.benchmark; }
bool show_used_paths(void) { return g_flags.show_used_paths; }
bool show_all_paths(void) { return g_flags.show_all_paths; }
