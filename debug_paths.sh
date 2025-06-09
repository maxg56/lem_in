
#!/bin/bash

# Génère le fichier d'entrée
./generator_linux --flow-thousand > ts

# Récupère la ligne attendue (prendre seulement la première occurrence)
EXPECTED=$(grep "#Here is the number of lines required:" ts | head -1 | cut -d':' -f2 | tr -d ' \n')

# Exécute lem-in avec l'entrée, et compte les lignes de sortie
ACTUAL=$(./lem-in < ts | wc -l)

# Affiche les deux valeurs
echo "Expected: $EXPECTED"
echo "Actual: $ACTUAL"

# Teste si c’est correct
if [ "$ACTUAL" -eq "$EXPECTED" ]; then
	echo "✅ Test passed"
else
	echo "❌ Test failed"
fi
