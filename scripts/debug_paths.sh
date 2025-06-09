#!/bin/bash

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Change to project root
cd "$PROJECT_ROOT" || exit 1

# Génère le fichier d'entrée
./scripts/generator_linux --big-superposition > ts

# Récupère la ligne attendue (prendre seulement la première occurrence)
EXPECTED=$(grep "#Here is the number of lines required:" ts | head -1 | cut -d':' -f2 | tr -d ' \n')

# Exécute lem-in avec l'entrée, et compte les lignes de sortie
ACTUAL=$(./lem-in < ts | wc -l)

# Affiche les deux valeurs
echo "Expected: $EXPECTED"
echo "Actual: $ACTUAL"

# Teste si c’est correct
if [ "$ACTUAL" -le "$EXPECTED" ]; then
	echo "✅ Test passed"
else
	echo "❌ Test failed"
fi

