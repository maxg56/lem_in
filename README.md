# Lem-in

Algorithme de recherche de chemins pour faire passer des fourmis d'une salle de départ à une salle d'arrivée.

## Structure du projet

- `src/` - Code source principal
  - `algo/` - Algorithmes de pathfinding et optimisation
  - `parsigne/` - Parsing des fichiers d'entrée
  - `utils/` - Utilitaires et fonctions d'aide
- `include/` - Headers
- `libft/` - Bibliothèque libft
- `tests/` - Tests unitaires
- `maps/` - Fichiers de cartes de test
- `scripts/` - Scripts utilitaires pour tests et benchmarks
- `visu-hex/` - Visualisateur web
- `docs/` - Documentation

## Compilation

```bash
make          # Compile le projet
make clean    # Nettoie les objets
make fclean   # Nettoie tout
make re       # Recompile tout
```

## Tests

```bash
cd tests && make    # Compile les tests
./lem_in_tests      # Lance les tests
```

## Scripts utilitaires

- `scripts/comprehensive_tester.sh` - Tests complets
- `scripts/performance_evaluator.sh` - Évaluation de performance
- `scripts/quick_performance_test.sh` - Tests rapides
- `scripts/debug_paths.sh` - Debug des chemins
- `scripts/cleanup.sh` - Nettoyage des fichiers temporaires
