# Rush01 - Skyscrapers Solver

## Description
Ce programme résout le puzzle "Skyscrapers" (gratte-ciels) en utilisant un algorithme de backtracking. Le puzzle consiste à placer des nombres de 1 à 4 dans une grille 4x4 de manière à ce que :
- Chaque ligne contienne les nombres 1 à 4 sans répétition
- Chaque colonne contienne les nombres 1 à 4 sans répétition
- Les contraintes de visibilité depuis chaque bord soient respectées

## Compilation
```bash
gcc -Wall -Wextra -Werror solver.c -o solver
```

## Utilisation
```bash
./solver "col1up col2up col3up col4up col1down col2down col3down col4down row1left row2left row3left row4left row1right row2right row3right row4right"
```

Les 16 valeurs doivent être des nombres entre 1 et 4, séparés par des espaces.

### Ordre des contraintes :
1. Les 4 premières valeurs : vues depuis le haut (colonnes 1 à 4)
2. Les 4 suivantes : vues depuis le bas (colonnes 1 à 4)
3. Les 4 suivantes : vues depuis la gauche (lignes 1 à 4)
4. Les 4 dernières : vues depuis la droite (lignes 1 à 4)

## Exemple
```bash
./solver "4 3 2 1 1 2 2 2 4 3 2 1 1 2 2 2"
```

Résultat :
```
1 2 3 4
2 3 4 1
3 4 1 2
4 1 2 3
```

## Gestion d'erreur
Le programme affiche "Error" dans les cas suivants :
- Nombre d'arguments incorrect
- Valeurs non comprises entre 1 et 4
- Format d'entrée invalide
- Aucune solution possible avec les contraintes données

## Améliorations apportées
- Lecture des contraintes depuis les arguments du programme
- Noms de variables explicites et commentaires détaillés
- Structure de code conforme à la norme 42
- Gestion d'erreur complète
- Validation des entrées utilisateur
