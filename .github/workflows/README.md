# 🚀 GitHub Actions pour LEM-IN

Ce répertoire contient les workflows GitHub Actions pour automatiser les tests, la qualité du code, les releases et les benchmarks de performance.

## 📁 Workflows Disponibles

### 🧪 `tests.yml` - Suite de Tests Principale
**Déclencheurs :** Push sur `main`/`develop`, Pull Requests

**Fonctionnalités :**
- ✅ Exécution complète des 116 tests
- 🔍 Tests de fuites mémoire avec Valgrind
- 🧪 Tests par catégorie (graph, edge, search, validation, parsing)
- 🔧 Tests avec différents compilateurs (GCC, Clang)
- ⚡ Tests de performance
- 📊 Génération de rapports détaillés

**Artefacts générés :**
- `test-report` - Rapport complet des tests
- `valgrind-output` - Analyse mémoire détaillée
- `performance-report` - Métriques de performance

### 🔍 `quality.yml` - Qualité du Code
**Déclencheurs :** Push sur `main`/`develop`, Pull Requests

**Fonctionnalités :**
- 📏 Vérification Norminette (style 42)
- 🔬 Analyse statique avec cppcheck
- ⚠️ Compilation avec warnings stricts
- 📚 Vérification de la documentation
- 🔒 Audit de sécurité basique

**Contrôles effectués :**
```bash
# Style et normes
norminette src/ include/ libft/

# Analyse statique
cppcheck --enable=all src/ include/

# Compilation stricte
gcc -Wall -Wextra -Werror -Wpedantic

# Sécurité
grep -r "strcpy\|strcat\|sprintf" # Fonctions dangereuses
```

### 🚀 `release.yml` - Gestion des Releases
**Déclencheurs :** Tags `v*`, Déclenchement manuel

**Fonctionnalités :**
- 🏗️ Build optimisé pour la release
- 🧪 Validation complète avant release
- 📦 Création d'archives (.tar.gz, .zip)
- 📊 Génération de statistiques de release
- 🎉 Création automatique de GitHub Release
- 📚 Déploiement de documentation

**Packages créés :**
- `lem-in-v1.0.0.tar.gz` - Archive complète
- `lem-in-v1.0.0.zip` - Archive ZIP
- Documentation web déployée

### ⚡ `benchmarks.yml` - Performance & Benchmarks
**Déclencheurs :** Quotidien (2h UTC), Push sur `main`, Manuel

**Fonctionnalités :**
- ⚡ Benchmarks de performance avec `hyperfine`
- 🔍 Analyse mémoire détaillée
- 📊 Tests de stress (50 exécutions)
- 📈 Comparaison historique
- 💬 Commentaires automatiques sur PR

**Métriques surveillées :**
- Temps d'exécution des tests
- Utilisation mémoire peak
- Consistance des résultats
- Performance par catégorie

## 🎯 Status Badges

Ajoutez ces badges à votre README principal :

```markdown
![Tests](https://github.com/YOUR_USERNAME/lem-in/actions/workflows/tests.yml/badge.svg)
![Quality](https://github.com/YOUR_USERNAME/lem-in/actions/workflows/quality.yml/badge.svg)
![Performance](https://github.com/YOUR_USERNAME/lem-in/actions/workflows/benchmarks.yml/badge.svg)
```

## 🔧 Configuration

### Variables d'Environnement
Aucune configuration spéciale requise - tous les workflows utilisent des outils standards.

### Secrets GitHub
- `GITHUB_TOKEN` - Automatiquement fourni par GitHub
- Aucun secret supplémentaire requis

### Permissions
Les workflows nécessitent :
- `contents: read` - Lecture du code
- `actions: write` - Upload d'artefacts
- `pages: write` - Déploiement documentation (release)

## 📊 Rapports Générés

### Rapport de Tests
```
🏁 RÉSULTATS FINAUX
==================================================
[PASS] TOUS LES TESTS RÉUSSISSENT ! (116/116)
✅ Graph Tests: 34/34
✅ Edge Management: 12/12
✅ Search & Navigation: 27/27
✅ Validation: 17/17
✅ Parsing: 26/26
==================================================
```

### Rapport de Performance
```
Benchmark #1: make test
  Time (mean ± σ):     234.2 ms ±  12.8 ms
  Range (min … max):   220.1 ms … 251.3 ms
```

### Analyse Mémoire
```
==SUMMARY==
definitely lost: 0 bytes in 0 blocks
indirectly lost: 0 bytes in 0 blocks
possibly lost: 0 bytes in 0 blocks
✅ No memory leaks detected!
```

## 🚦 Workflow de Développement

### Pour les Développeurs

1. **Push/PR sur develop :**
   - ✅ Tests automatiques
   - 🔍 Vérification qualité
   - 📊 Rapport de performance

2. **Merge sur main :**
   - ✅ Tests complets
   - ⚡ Benchmarks de performance
   - 📈 Mise à jour historique

3. **Tag de release :**
   - 🚀 Release automatique
   - 📦 Packages générés
   - 📚 Documentation déployée

### Commandes Locales Équivalentes

```bash
# Reproduire les tests CI localement
make -C tests test

# Vérifier la qualité
norminette src/ include/
cppcheck src/ include/

# Tests mémoire
cd tests && valgrind ./lem_in_tests

# Benchmark performance
hyperfine 'make -C tests test'
```

## 🐛 Debugging des Workflows

### Logs Détaillés
Chaque workflow génère des logs détaillés visibles dans l'onglet Actions de GitHub.

### Artefacts de Debug
En cas d'échec, consultez les artefacts :
- `test-report` - Détails des tests échoués
- `valgrind-output` - Problèmes mémoire
- `cppcheck-report` - Problèmes de code

### Tests Locaux
Reproduisez les problèmes localement :
```bash
# Même environnement que CI
docker run -it ubuntu:latest
apt-get update && apt-get install -y gcc make valgrind
# ... reproduire les étapes du workflow
```

## 📈 Évolutions Futures

### Améliorations Prévues
- 🔧 Tests sur multiple OS (Windows, macOS)
- 📊 Métriques de couverture de code
- 🚀 Déploiement automatique sur serveur de test
- 📱 Notifications Slack/Discord
- 🏗️ Matrix builds avec différentes versions GCC

### Intégrations Possibles
- 🔍 SonarQube pour analyse avancée
- 📊 Codecov pour couverture
- 🐛 Sentry pour monitoring erreurs
- 📈 Grafana pour métriques long terme

---

*Ces workflows évoluent avec le projet. Contribuez en proposant des améliorations !*
