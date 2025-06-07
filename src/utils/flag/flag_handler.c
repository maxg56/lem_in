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
    bool visual;           // --visual : mode visuel pour le web
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
    g_flags.visual = false;
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
            else if (ft_strcmp(argv[i], "--visual") == 0) {
                g_flags.visual = true;
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
    ft_putstr_fd("  --visual         Mode visuel pour interface web\n", 1);
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

// Getters pour les flags
bool is_verbose(void) { return g_flags.verbose; }
bool is_debug(void) { return g_flags.debug; }
bool show_paths(void) { return g_flags.show_paths; }
bool show_stats(void) { return g_flags.show_stats; }
bool is_help(void) { return g_flags.help; }
bool is_quiet(void) { return g_flags.quiet; }
bool is_benchmark(void) { return g_flags.benchmark; }
bool is_visual(void) { return g_flags.visual; }

// Fonction pour afficher en mode debug
void debug_printf(const char *format, ...) {
    if (is_debug()) {
        ft_putstr_fd("[DEBUG] ", 2);
        // Vous pouvez utiliser ft_printf si disponible, sinon ft_putstr_fd
        ft_putstr_fd((char*)format, 2);
    }
}

// Fonction pour afficher en mode verbose
void verbose_printf(const char *format, ...) {
    if (is_verbose() && !is_quiet()) {
        ft_putstr_fd("[INFO] ", 1);
        ft_putstr_fd((char*)format, 1);
    }
}
