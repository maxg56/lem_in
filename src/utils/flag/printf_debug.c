#include "lem-in.h"

// Fonction pour afficher en mode debug
void debug_printf(const char *format, ...) {
    if (is_debug()) {
        ft_putstr_fd("[DEBUG] ", 2);
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