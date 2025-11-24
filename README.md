# 🔬 Projet ICC: Système de Reconnaissance d'Empreintes Digitales

[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/status-in%20development-yellow.svg)]()
[![EPFL](https://img.shields.io/badge/EPFL-ICC%20(SV)-red.svg)]()
[![Private](https://img.shields.io/badge/repo-private-red.svg)]()

> **⚠️ Projet Académique Privé**  
> Ce repository est destiné au travail collaboratif entre binômes dans le cadre du cours ICC à l'EPFL.  
> Tout le code doit être écrit personnellement. Ce README sert uniquement de guide méthodologique.

---

### IMPORTANT POUR LES COMMANDE GIT :
- [Commande git pour Eulalie](#antisèches-cheat-sheets)
- [Encore...](#gestion-des-conflits)
- [Ah et aussi :](#)

## 📖 Table des Matières

- [Vue d'Ensemble](#-vue-densemble)
- [Archi du Projet](#archi-du-projet)
- [Checklist Complète de Développement](#-checklist-complète-de-développement)
- [Méthodologie de Travail](#-méthodologie-de-travail)
- [Installation et Configuration](#-installation-et-configuration)
- [Tests et Validation](#-tests-et-validation)
- [Gestion de Version](#-gestion-de-version)
- [Ressources et Documentation](#-ressources-et-documentation)

---

## 🎯 Vue d'Ensemble

### Description du Projet

Système de reconnaissance et comparaison d'empreintes digitales implémentant un pipeline complet :
- **Squelettisation** : Réduction des traits à une épaisseur d'un pixel
- **Extraction de minuties** : Identification des points caractéristiques (terminaisons et bifurcations)
- **Comparaison** : Matching avec transformations géométriques (rotation, translation)

### Objectifs Pédagogiques

✅ Manipulation avancée de structures de données (tableaux 2D)  
✅ Algorithmes itératifs et conditions complexes  
✅ Programmation modulaire en C++  
✅ Tests et validation rigoureuse  
✅ Travail collaboratif sur projet technique  

### Technologies Utilisées

- **Langage** : C++11
- **IDE** : Qt Creator (recommandé)
- **Build System** : CMake / qmake
- **Versioning** : Git
- **Tests** : Framework de test fourni

### Fonctionnalités Clés

| Module | Fonctionnalité | Description |
|--------|---------------|-------------|
| 🖼️ **Prétraitement** | Squelettisation | Algorithme itératif de thinning |
| 🎯 **Extraction** | Détection minuties | Terminaisons (1 transition) et bifurcations (3 transitions) |
| 📐 **Orientation** | Calcul d'angle | Régression linéaire + analyse perpendiculaire |
| 🔄 **Transformation** | Rotation/Translation | Superposition géométrique |
| ✓ **Matching** | Comparaison | Force brute avec seuils de tolérance |

---

## Archi du projet

### Structure des Fichiers
```
projet-empreintes/
│
├── 📁 src/
│   ├── fingerprint.cpp          # 🔴 FICHIER À COMPLÉTER (seul fichier rendu)
│   ├── fingerprint.hpp          # Structures et déclarations (ne pas modifier)
│   ├── helper.cpp               # Utilitaires fournis (lecture d'image)
│   ├── helper.hpp               # Headers des utilitaires
│   ├── utils.hpp                # Utilitaires système
│   └── main.cpp                 # Tests (modifiable pour debug)
│
├── 📁 resources/
│   ├── fingerprints/            # Images d'empreintes test
│   │   ├── 1_1.png             # Doigt 1, capture 1
│   │   ├── 1_2.png             # Doigt 1, capture 2
│   │   ├── 2_1.png             # Doigt 2, capture 1
│   │   └── ...
│   └── test_outputs/            # Résultats de référence
│       ├── skeleton_1_1.png
│       ├── minutiae_1_1.png
│       └── ...
│
├── 📁 docs/                     # Documentation (à créer)
│   ├── notes_techniques.md
│   ├── decisions.md
│   └── bugs_connus.md
│
├── 📄 README.md                 # Ce fichier
├── 📄 fingerprints_fr.pdf       # Énoncé officiel du projet
├── 📄 .gitignore               # Fichiers à ignorer
└── 🔧 CMakeLists.txt / .pro     # Configuration de compilation
```

### Structures de Données Clés

#### Types Fournis

| Type | Description | Utilisation |
|------|-------------|-------------|
| `BinaryImage` | `vector<vector<bool>>` | Images noir et blanc |
| `ARGBImage` | `vector<vector<int>>` | Images couleur (debug) |
| `Minutia` | `struct {row, column, orientation}` | Point caractéristique |

#### Constantes Importantes
```cpp
// Définies dans fingerprint.hpp (ne pas modifier)

ORIENTATION_DISTANCE      // Distance pour calcul orientation (ex: 20)
DISTANCE_THRESHOLD        // Seuil distance pour matching (ex: 5)
ORIENTATION_THRESHOLD     // Seuil orientation pour matching (ex: 20°)
FOUND_THRESHOLD          // Nombre min de minuties correspondantes (ex: 12)
MATCH_ANGLE_OFFSET       // Tolérance rotation (ex: 2°)
```

### Conventions de Coordonnées
```
⚠️ ATTENTION : Système de coordonnées image

(0,0) ───────────────────> X (colonnes)
 │
 │    P7  P0  P1
 │    P6  P   P2
 │    P5  P4  P3
 │
 ▼
 Y (lignes)

- Origine en HAUT À GAUCHE
- Axe Y vers le BAS
- image[row][column]
- image[0][0] = coin supérieur gauche
- image[height-1][width-1] = coin inférieur droit
```

---

## ✅ Checklist Complète de Développement

> 📝 **Note** : Cocher les cases au fur et à mesure de l'avancement  
> 💡 **Conseil** : Tester après chaque fonction implémentée

### 📦 Phase 0 : Configuration et Préparation

#### 0.1. Setup Environnement
- [ ] Installer Qt Creator ou configurer compilateur C++11
- [ ] Cloner le repository git privé
- [ ] Configurer les accès pour les deux membres
- [ ] Ouvrir le projet dans l'IDE
- [ ] Vérifier la compilation du code fourni (avec NotImplemented)
- [ ] Exécuter main.cpp pour vérifier les dépendances
- [ ] Créer la structure de branches git (voir section Gestion de Version)

#### 0.2. Étude du Cahier des Charges
- [ ] Lire intégralement `fingerprints_fr.pdf`
- [ ] Comprendre le pipeline : prétraitement → squelettisation → extraction → comparaison
- [ ] Étudier les figures d'exemple (terminaison, bifurcation, noyau, delta)
- [ ] Comprendre l'algorithme de squelettisation (conditions étapes 1 et 2)
- [ ] Comprendre le calcul d'orientation (régression linéaire)
- [ ] Comprendre l'algorithme de matching (force brute)

#### 0.3. Analyse des Structures Fournies
- [ ] Examiner `fingerprint.hpp` : tous les prototypes de fonctions
- [ ] Comprendre la structure `Minutia`
- [ ] Lire toutes les constantes définies
- [ ] Examiner `helper.hpp` : fonctions de lecture/écriture d'images
- [ ] Identifier les fonctions utilitaires disponibles :
  - [ ] `to_int()` et `to_size_t()` pour conversions
  - [ ] `radians_to_degrees()` et `degrees_to_radians()`
  - [ ] `read_binary()` et `write_binary()`
  - [ ] Fonctions de debug : `add_line()`, `add_circle()`, `draw_minutia()`

#### 0.4. Planification
- [ ] Créer les issues GitHub pour chaque fonction
- [ ] Définir les conventions de code (nommage, commentaires)
- [ ] Établir le calendrier de travail
- [ ] Définir les points de synchronisation (daily standup)

---

### 🧩 Partie 1 : Squelettisation d'Image Binaire

> **Objectif** : Réduire tous les traits de l'empreinte à une épaisseur d'un pixel  

---

#### 1.1. Fonction `get_neighbours`

**Signature :**
```cpp
std::vector<bool> get_neighbours(const BinaryImage &binary_image, 
                                 size_t row, size_t column);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer une branche : `git checkout -b feature/get-neighbours`
  - [ ] Localiser le prototype dans `fingerprint.hpp`
  - [ ] Ouvrir `fingerprint.cpp`
  - [ ] Trouver la fonction avec `NotImplemented();`
  - [ ] Supprimer la ligne `NotImplemented();`

- [ ] **Validation des paramètres d'entrée**
  - [ ] Vérifier que `row` est dans les limites de l'image
  - [ ] Vérifier que `column` est dans les limites de l'image
  - [ ] Si coordonnées invalides : retourner vecteur vide

- [ ] **Création du vecteur de retour**
  - [ ] Créer un `std::vector<bool>` de taille 8
  - [ ] Initialiser tous les éléments à `false` par défaut

- [ ] **Extraction des 8 voisins dans l'ordre P0→P7**
  - [ ] P0 : pixel du haut `[row-1][column]`
  - [ ] P1 : pixel haut-droite `[row-1][column+1]`
  - [ ] P2 : pixel droite `[row][column+1]`
  - [ ] P3 : pixel bas-droite `[row+1][column+1]`
  - [ ] P4 : pixel du bas `[row+1][column]`
  - [ ] P5 : pixel bas-gauche `[row+1][column-1]`
  - [ ] P6 : pixel gauche `[row][column-1]`
  - [ ] P7 : pixel haut-gauche `[row-1][column-1]`

- [ ] **Gestion des pixels hors limites**
  - [ ] Pour P0, P1, P7 : vérifier `row > 0`
  - [ ] Pour P3, P4, P5 : vérifier `row < height-1`
  - [ ] Pour P5, P6, P7 : vérifier `column > 0`
  - [ ] Pour P1, P2, P3 : vérifier `column < width-1`
  - [ ] Si pixel hors limites : considérer comme `false` (blanc)

- [ ] **Tests unitaires**
  - [ ] Compiler le projet
  - [ ] Exécuter `test_get_neighbours_1()` dans main
  - [ ] Vérifier que le test affiche "OK"
  - [ ] Exécuter `test_get_neighbours_2()`
  - [ ] Vérifier que le test affiche "OK"

- [ ] **Tests manuels supplémentaires**
  - [ ] Tester avec un pixel au centre de l'image (tous voisins existent)
  - [ ] Tester avec un pixel en coin (0,0) → plusieurs voisins hors limites
  - [ ] Tester avec un pixel en bordure → certains voisins hors limites
  - [ ] Tester avec coordonnées invalides → vecteur vide

- [ ] **Finalisation**
  - [ ] Commit : `git commit -m "Implement get_neighbours function"`
  - [ ] Push : `git push origin feature/get-neighbours`
  - [ ] Créer Pull Request pour review du binôme

---

#### 1.2. Fonction `black_neighbours`

**Signature :**
```cpp
unsigned int black_neighbours(std::vector<bool> neighbours);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/black-neighbours`
  - [ ] Localiser et ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Logique de comptage**
  - [ ] Initialiser un compteur à 0
  - [ ] Parcourir le vecteur `neighbours` avec une boucle
  - [ ] Pour chaque élément `true` (pixel noir) : incrémenter le compteur
  - [ ] Retourner le compteur final

- [ ] **Validation du résultat**
  - [ ] S'assurer que le résultat est entre 0 et 8 inclus
  - [ ] Gérer le cas d'un vecteur vide (retourner 0)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_black_neighbours()` → doit afficher "OK"
  - [ ] Test manuel : vecteur vide → 0
  - [ ] Test manuel : tous noirs (8 true) → 8
  - [ ] Test manuel : 3 noirs → 3

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 1.3. Fonction `transitions`

**Signature :**
```cpp
unsigned int transitions(std::vector<bool> neighbours);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/transitions`
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Logique de comptage des transitions blanc→noir**
  - [ ] Initialiser compteur à 0
  - [ ] Boucle sur les 8 positions (i = 0 à 7)
  - [ ] Pour chaque position i :
    - [ ] Calculer position suivante : `next = (i + 1) % 8` (pour boucler)
    - [ ] Si `neighbours[i]` est blanc ET `neighbours[next]` est noir : incrémenter
  - [ ] Retourner le compteur

- [ ] **Comprendre la séquence circulaire**
  - [ ] P0 → P1 → P2 → P3 → P4 → P5 → P6 → P7 → P0 (retour au début)
  - [ ] Le modulo 8 gère automatiquement le bouclage P7→P0

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_transitions()` → "OK"
  - [ ] Test manuel : terminaison (figure 17 du PDF) → 1 transition
  - [ ] Test manuel : bifurcation (figure 18 du PDF) → 3 transitions
  - [ ] Test manuel : tous blancs → 0
  - [ ] Test manuel : alternance parfaite blanc/noir → 4 transitions

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 1.4. Fonction `identical`

**Signature :**
```cpp
bool identical(const BinaryImage &binary_image_1,const BinaryImage &binary_image_2);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/identical`
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Vérification préliminaire des dimensions**
  - [ ] Comparer le nombre de lignes : `size()`
  - [ ] Si différent : retourner `false` immédiatement
  - [ ] Comparer le nombre de colonnes : `[0].size()`
  - [ ] Si différent : retourner `false` immédiatement

- [ ] **Comparaison pixel par pixel**
  - [ ] Double boucle imbriquée (lignes et colonnes)
  - [ ] Pour chaque pixel :
    - [ ] Comparer `image_1[row][col]` avec `image_2[row][col]`
    - [ ] Si différent : retourner `false` immédiatement (early exit)

- [ ] **Cas où toutes les comparaisons réussissent**
  - [ ] Retourner `true` (images identiques)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_identical_true()` → "OK"
  - [ ] Exécuter `test_identical_false()` → "OK"
  - [ ] Test manuel : deux images vides → true
  - [ ] Test manuel : dimensions différentes → false
  - [ ] Test manuel : 1 pixel différent → false

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 1.5. Fonction `thinning_step`

**Signature :**
```cpp
BinaryImage thinning_step(const BinaryImage &binary_image, int step);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/thinning-step`
  - [ ] Relire attentivement la section 3.3 du PDF (conditions des deux étapes)
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Créer une copie de l'image d'entrée (ne pas modifier l'originale)
  - [ ] Cette copie sera modifiée et retournée

- [ ] **Parcours de tous les pixels**
  - [ ] Double boucle sur toutes les lignes
  - [ ] Double boucle sur toutes les colonnes

- [ ] **Vérification Condition 1 : Pixel noir**
  - [ ] Vérifier que le pixel actuel est noir dans l'image ORIGINALE
  - [ ] Si blanc : passer au pixel suivant (continue)

- [ ] **Vérification Condition 2 : Voisins valides**
  - [ ] Appeler `get_neighbours()` sur l'image ORIGINALE
  - [ ] Vérifier que le vecteur retourné n'est pas vide
  - [ ] Si vide : passer au pixel suivant

- [ ] **Vérification Condition 3 : Nombre de voisins noirs**
  - [ ] Appeler `black_neighbours()` sur le vecteur de voisins
  - [ ] Vérifier : `2 ≤ black_neighbours() ≤ 6`
  - [ ] Si non respecté : passer au pixel suivant

- [ ] **Vérification Condition 4 : Nombre de transitions**
  - [ ] Appeler `transitions()` sur le vecteur de voisins
  - [ ] Vérifier : `transitions() == 1`
  - [ ] Si différent : passer au pixel suivant

- [ ] **Vérification Conditions 5 et 6 (spécifiques à l'étape)**
  - [ ] **Si step == 0 (Étape 1) :**
    - [ ] Condition 5 : Au moins un de P0, P2, P4 doit être blanc
    - [ ] Condition 6 : Au moins un de P2, P4, P6 doit être blanc
  - [ ] **Si step == 1 (Étape 2) :**
    - [ ] Condition 5 : Au moins un de P0, P2, P6 doit être blanc
    - [ ] Condition 6 : Au moins un de P0, P4, P6 doit être blanc
  - [ ] Si conditions non respectées : passer au pixel suivant

- [ ] **Suppression du pixel**
  - [ ] Si TOUTES les conditions sont vraies : mettre le pixel à `false` (blanc) dans la copie

- [ ] **Retour**
  - [ ] Retourner l'image modifiée (copie)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_thinning_step_single_pixel()` → "OK"
  - [ ] Test manuel : image avec un seul pixel noir → doit rester noir
  - [ ] Test manuel : petite image avec traits de 3 pixels d'épaisseur → vérifier l'amincissement

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review détaillée (fonction complexe)

---

#### 1.6. Fonction `thin`

**Signature :**
```cpp
BinaryImage thin(const BinaryImage &binary_image);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/thin`
  - [ ] Comprendre l'algorithme itératif (section 3.3 du PDF)
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Créer une copie de l'image d'entrée
  - [ ] Cette copie sera itérativement modifiée

- [ ] **Boucle principale d'itération**
  - [ ] Initialiser une variable booléenne `changed = true`
  - [ ] Boucle `while (changed)` :

- [ ] **Sauvegarde de l'état avant les deux étapes**
  - [ ] À chaque itération : sauvegarder l'image actuelle dans une variable temporaire

- [ ] **Application des deux étapes de squelettisation**
  - [ ] Appliquer l'étape 1 : `result = thinning_step(result, 0)`
  - [ ] Appliquer l'étape 2 : `result = thinning_step(result, 1)`

- [ ] **Détection des changements**
  - [ ] Comparer l'image sauvegardée avec l'image après les deux étapes
  - [ ] Utiliser la fonction `identical()`
  - [ ] Si identiques : `changed = false` (sortie de boucle)
  - [ ] Si différentes : `changed = true` (nouvelle itération)

- [ ] **Retour**
  - [ ] Retourner l'image finale squelettisée

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_thin()` → "OK"
  - [ ] Exécuter `test_draw_skeleton()` pour toutes les images fournies
  - [ ] Vérifier que toutes les images générées sont identiques aux références

- [ ] **Validation visuelle**
  - [ ] Ouvrir `skeleton_1_1.png` généré
  - [ ] Comparer avec `resources/test_outputs/skeleton_1_1.png`
  - [ ] Vérifier : traits d'épaisseur 1 pixel, structure préservée
  - [ ] S'il y a des pixels en trop : ils apparaissent en rouge
  - [ ] Répéter pour `skeleton_1_2.png` et `skeleton_2_1.png`

- [ ] **Finalisation Partie 1**
  - [ ] Commit et push
  - [ ] Pull Request finale
  - [ ] Review complète avec binôme
  - [ ] Merge dans main après validation
  - [ ] ✨ **CHECKPOINT : Partie 1 terminée !**

---

### 🎯 Partie 2 : Extraction et Orientation des Minuties

> **Objectif** : Localiser les minuties et calculer leur orientation (0-359°)  
> **Assignation suggérée** : Membre 2 (ou après Partie 1 pour Membre 1)

#### 2.1. Fonction `connected_pixels`

**Signature :**
```cpp
BinaryImage connected_pixels(const BinaryImage &binary_image,
                             size_t row,
                             size_t column,
                             unsigned int distance);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/connected-pixels`
  - [ ] Relire la section 3.4.1 du PDF
  - [ ] Comprendre le principe de propagation (algorithme itératif)
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Créer une image binaire de même taille (initialisée à `false` partout)
  - [ ] Cette image représentera les pixels connectés

- [ ] **Marquer le pixel de départ (la minutie)**
  - [ ] Mettre le pixel `[row][column]` à `true` dans l'image de pixels connectés

- [ ] **Calcul des limites du carré de recherche**
  - [ ] Calculer `min_row` : maximum entre 0 et (row - distance)
  - [ ] Calculer `max_row` : minimum entre (height-1) et (row + distance)
  - [ ] Calculer `min_col` : maximum entre 0 et (column - distance)
  - [ ] Calculer `max_col` : minimum entre (width-1) et (column + distance)
  - [ ] ⚠️ Attention aux conversions `size_t` ↔ `int` (utiliser `to_int()`)

- [ ] **Algorithme de propagation itérative**
  - [ ] Initialiser `bool changed = true`
  - [ ] Boucle `while (changed)` :
    - [ ] Mettre `changed = false` au début de chaque itération
    - [ ] Double boucle sur la zone [min_row:max_row][min_col:max_col]
    - [ ] Pour chaque pixel dans cette zone :

- [ ] **Conditions pour ajouter un pixel à l'ensemble connecté**
  - [ ] Vérifier : pixel pas déjà marqué comme connecté (sinon continue)
  - [ ] Vérifier : pixel est noir dans l'image ORIGINALE (sinon continue)
  - [ ] Vérifier : au moins un des 8 voisins directs est déjà marqué connecté
    - [ ] Parcourir les 8 directions
    - [ ] Si au moins un voisin est `true` dans l'image connectée → OK
  - [ ] Si toutes conditions OK :
    - [ ] Marquer le pixel comme connecté
    - [ ] Mettre `changed = true` (il faudra une autre itération)

- [ ] **Sortie de boucle**
  - [ ] Quand `changed = false` : aucun nouveau pixel n'a été ajouté
  - [ ] Tous les pixels connectés ont été trouvés

- [ ] **Retour**
  - [ ] Retourner l'image binaire des pixels connectés

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_connected_pixels_1()` → "OK"
  - [ ] Exécuter `test_connected_pixels_2()` → "OK"
  - [ ] Exécuter `test_connected_pixels_3()` → "OK"
  - [ ] Vérifier visuellement les exemples (figure 13 du PDF)

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 2.2. Fonction `compute_slope`

**Signature :**
```cpp
double compute_slope(const BinaryImage &connected_pixels,
                    size_t row,
                    size_t column);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/compute-slope`
  - [ ] Relire la section 3.4.2 et l'annexe A.2 du PDF (régression linéaire)
  - [ ] Comprendre les formules mathématiques
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation des sommes**
  - [ ] Initialiser `sum_xy = 0.0` (somme des produits x·y)
  - [ ] Initialiser `sum_x2 = 0.0` (somme des carrés de x)
  - [ ] Initialiser `sum_y2 = 0.0` (somme des carrés de y)

- [ ] **Parcours de tous les pixels connectés**
  - [ ] Double boucle sur toute l'image `connected_pixels`
  - [ ] Pour chaque pixel marqué `true` :

- [ ] **Calcul des coordonnées relatives**
  - [ ] Calculer `x = col - column` (décalage horizontal)
  - [ ] Calculer `y = row_minutia - row_pixel` ⚠️ **ATTENTION AU SIGNE !**
    - [ ] Rappel : repère image (origine haut-gauche) ≠ repère mathématique
    - [ ] Le y est inversé : `y = row_minutia - row_pixel`

- [ ] **Accumulation des sommes**
  - [ ] `sum_xy += x * y`
  - [ ] `sum_x2 += x * x`
  - [ ] `sum_y2 += y * y`

- [ ] **Cas particulier : ligne verticale**
  - [ ] Si `sum_x2 == 0` (tous les pixels alignés verticalement)
  - [ ] Retourner `std::numeric_limits<double>::infinity()`
  - [ ] (Représente une pente infinie)

- [ ] **Calcul de la pente selon la formule**
  - [ ] Si `sum_x2 >= sum_y2` : ligne plus horizontale que verticale
    - [ ] Formule : `slope = sum_xy / sum_x2`
  - [ ] Sinon : ligne plus verticale qu'horizontale
    - [ ] Formule : `slope = sum_y2 / sum_xy`

- [ ] **Retour**
  - [ ] Retourner la pente calculée (type `double`)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_compute_slope_horizontal()` → "OK"
  - [ ] Test manuel : ligne horizontale → slope ≈ 0
  - [ ] Test manuel : ligne verticale → slope = infinity
  - [ ] Test manuel : ligne à 45° → slope ≈ 1 (ou proche)

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 2.3. Fonction `compute_angle`

**Signature :**
```cpp
double compute_angle(const BinaryImage &connected_pixels,
                    size_t row,
                    size_t column,
                    double slope);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/compute-angle`
  - [ ] Relire la section 3.4.3 du PDF
  - [ ] Comprendre le problème de l'ambiguïté du sens (figure 15)
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Gestion du cas particulier : pente infinie**
  - [ ] Si `std::isinf(slope)` (ligne verticale)
  - [ ] Compter les pixels au-dessus et en-dessous de la minutie
    - [ ] Parcourir tous les pixels connectés
    - [ ] Pixels avec `row < row_minutia` : au-dessus
    - [ ] Pixels avec `row > row_minutia` : en-dessous
  - [ ] Si plus de pixels au-dessus : retourner `M_PI / 2` (90°)
  - [ ] Sinon : retourner `-M_PI / 2` (-90°)

- [ ] **Calcul de l'angle initial avec arctan**
  - [ ] `angle = std::atan(slope)`
  - [ ] Résultat : angle entre -π/2 et π/2 (en radians)

- [ ] **Calcul de l'équation de la droite perpendiculaire**
  - [ ] Équation : `y = -1/slope * x`
  - [ ] Cette droite sert à séparer l'espace en deux zones

- [ ] **Comptage des pixels au-dessus/en-dessous de la perpendiculaire**
  - [ ] Initialiser `above = 0` et `below = 0`
  - [ ] Parcourir tous les pixels connectés
  - [ ] Pour chaque pixel :
    - [ ] Calculer coordonnées relatives `x` et `y`
    - [ ] Tester : `y >= -1.0/slope * x`
      - [ ] Si vrai : pixel au-dessus → `above++`
      - [ ] Sinon : pixel en-dessous → `below++`

- [ ] **Ajustement de l'angle selon la règle (section 3.4.3)**
  - [ ] Si `angle > 0` ET `below > above` : ajouter π à l'angle
  - [ ] Si `angle < 0` ET `above > below` : ajouter π à l'angle
  - [ ] Cela résout l'ambiguïté du sens

- [ ] **Retour**
  - [ ] Retourner l'angle en radians (entre -π et π après ajustement)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_compute_angle_horizontal()` → "OK"
  - [ ] Test manuel : ligne horizontale vers la droite → 0
  - [ ] Test manuel : ligne verticale vers le haut → π/2
  - [ ] Test manuel : ligne à 45° → π/4 (environ)

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 2.4. Fonction `compute_orientation`

**Signature :**
```cpp
int compute_orientation(const BinaryImage &binary_image,
                       size_t row,
                       size_t column,
                       unsigned int distance);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/compute-orientation`
  - [ ] Relire la section 3.4.4 du PDF
  - [ ] Comprendre : cette fonction orchestre les 3 précédentes
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Étape 1 : Pixels connectés**
  - [ ] Appeler `connected_pixels(binary_image, row, column, distance)`
  - [ ] Stocker le résultat dans une variable

- [ ] **Étape 2 : Calcul de la pente**
  - [ ] Appeler `compute_slope(connected_pixels, row, column)`
  - [ ] Stocker la pente dans une variable

- [ ] **Étape 3 : Calcul de l'angle**
  - [ ] Appeler `compute_angle(connected_pixels, row, column, slope)`
  - [ ] Stocker l'angle (en radians) dans une variable

- [ ] **Conversion en degrés**
  - [ ] Utiliser la fonction fournie `radians_to_degrees(angle)`
  - [ ] Stocker le résultat en `double`

- [ ] **Normalisation (0-359°)**
  - [ ] Si l'angle en degrés est négatif : ajouter 360
  - [ ] Assurer que le résultat est dans [0, 360[

- [ ] **Arrondi et conversion en entier**
  - [ ] Utiliser `std::round(degrees)`
  - [ ] Convertir avec `static_cast<int>(...)`

- [ ] **Retour**
  - [ ] Retourner l'orientation en degrés (entier de 0 à 359)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_orientation()` → "OK"
  - [ ] Vérifier que le résultat correspond à l'exemple fourni
  - [ ] Tester avec `distance = ORIENTATION_DISTANCE` (valeur de la constante)

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 2.5. Fonction `extract`

**Signature :**
```cpp
std::vector<Minutia> extract(const BinaryImage &binary_image);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/extract`
  - [ ] Relire la section 3.5 du PDF
  - [ ] Comprendre : terminaisons (1 transition) et bifurcations (3 transitions)
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Créer un vecteur vide : `std::vector<Minutia> minutiae;`
  - [ ] Obtenir hauteur et largeur de l'image

- [ ] **Parcours de l'image (sans les bords)**
  - [ ] Boucle sur les lignes : `row = 1` à `height-2` (inclus)
  - [ ] Boucle sur les colonnes : `col = 1` à `width-2` (inclus)
  - [ ] ⚠️ On exclut les bords pour garantir que chaque pixel a 8 voisins

- [ ] **Vérification : pixel noir**
  - [ ] Si le pixel actuel est blanc : passer au suivant (continue)

- [ ] **Extraction des voisins**
  - [ ] Appeler `get_neighbours(binary_image, row, col)`
  - [ ] Si le vecteur est vide : passer au suivant

- [ ] **Calcul du nombre de transitions**
  - [ ] Appeler `transitions(neighbours)`
  - [ ] Stocker le résultat

- [ ] **Identification terminaison ou bifurcation**
  - [ ] Si `transitions == 1` : c'est une terminaison
  - [ ] Si `transitions == 3` : c'est une bifurcation
  - [ ] Si ni l'un ni l'autre : passer au pixel suivant

- [ ] **Calcul de l'orientation**
  - [ ] Appeler `compute_orientation(binary_image, row, col, ORIENTATION_DISTANCE)`
  - [ ] ⚠️ Utiliser la constante `ORIENTATION_DISTANCE` définie dans le .hpp

- [ ] **Création de la structure Minutia**
  - [ ] Créer une instance de `Minutia`
  - [ ] Affecter `row`, `column`, `orientation`
  - [ ] Ajouter au vecteur avec `push_back()`

- [ ] **Retour**
  - [ ] Retourner le vecteur complet de toutes les minuties trouvées

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_extract_single_minutia()` → "OK"
  - [ ] Exécuter `test_with_skeleton()` → "OK"
  - [ ] Exécuter `draw_minutiae()` → génère des images avec cercles rouges

- [ ] **Validation visuelle**
  - [ ] Ouvrir `minutiae_1_1.png`
  - [ ] Vérifier : cercles rouges sur toutes les terminaisons
  - [ ] Vérifier : cercles rouges sur toutes les bifurcations
  - [ ] Vérifier : orientations cohérentes (flèches)
  - [ ] Répéter pour `minutiae_1_2.png` et `minutiae_2_1.png`

- [ ] **Finalisation Partie 2**
  - [ ] Commit et push
  - [ ] Pull Request finale
  - [ ] Review complète avec binôme
  - [ ] Merge dans main après validation
  - [ ] ✨ **CHECKPOINT : Partie 2 terminée !**

---

### 🔄 Partie 3 : Comparaison et Matching d'Empreintes

> **Objectif** : Déterminer si deux ensembles de minuties proviennent du même doigt  
> **Assignation suggérée** : Les deux membres ensemble (fonctions interdépendantes)

#### 3.1. Fonction `apply_rotation`

**Signature :**
```cpp
Minutia apply_rotation(const Minutia &minutia,
                      int center_row,
                      int center_column,
                      int rotation_in_degrees);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/apply-rotation`
  - [ ] Relire la section 3.7.1 du PDF (formules de rotation)
  - [ ] Comprendre les matrices de rotation 2D
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Conversion rotation en radians**
  - [ ] Utiliser `degrees_to_radians(rotation_in_degrees)`
  - [ ] Stocker dans une variable `theta`

- [ ] **Calcul des coordonnées relatives**
  - [ ] `x = minutia.column - center_column`
  - [ ] `y = center_row - minutia.row` ⚠️ **Attention : y inversé !**

- [ ] **Application de la matrice de rotation**
  - [ ] Formule : `newX = x * cos(theta) - y * sin(theta)`
  - [ ] Formule : `newY = x * sin(theta) + y * cos(theta)`
  - [ ] Utiliser `std::cos()` et `std::sin()` (prennent des radians)

- [ ] **Recalcul des coordonnées dans le repère image**
  - [ ] `newRow = center_row - newY`
  - [ ] `newCol = newX + center_column`

- [ ] **Arrondi et conversion en entiers**
  - [ ] Utiliser `std::round()` pour arrondir
  - [ ] Convertir avec `static_cast<int>()`

- [ ] **Calcul de la nouvelle orientation**
  - [ ] `newOrientation = (minutia.orientation + rotation_in_degrees) % 360`
  - [ ] Si le résultat est négatif : ajouter 360

- [ ] **Création et retour de la minutie transformée**
  - [ ] Créer une nouvelle structure `Minutia`
  - [ ] Affecter `row`, `column`, `orientation`
  - [ ] Retourner cette structure

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_apply_rotation()` → tous les sous-tests "OK"
  - [ ] Vérifier les 5 cas de test fournis

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 3.2. Fonction `apply_translation`

**Signature :**
```cpp
Minutia apply_translation(const Minutia &minutia,
                         int row_translation,
                         int column_translation);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/apply-translation`
  - [ ] Relire la section 3.7.1 du PDF
  - [ ] Comprendre : déplacement simple sans rotation
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Calcul des nouvelles coordonnées**
  - [ ] Formule : `newRow = minutia.row - row_translation`
  - [ ] Formule : `newCol = minutia.column - column_translation`

- [ ] **Orientation inchangée**
  - [ ] `newOrientation = minutia.orientation`

- [ ] **Création et retour de la minutie**
  - [ ] Créer une nouvelle structure `Minutia`
  - [ ] Affecter les nouvelles valeurs
  - [ ] Retourner la structure

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_apply_translation()` → tous les tests "OK"

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 3.3. Fonction `apply_transformation` (minutie unique)

**Signature :**
```cpp
Minutia apply_transformation(const Minutia &minutia,
                            int center_row,
                            int center_column,
                            int row_translation,
                            int column_translation,
                            int rotation_in_degrees);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/apply-transformation`
  - [ ] Comprendre : composition rotation PUIS translation
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Étape 1 : Application de la rotation**
  - [ ] Appeler `apply_rotation(minutia, center_row, center_column, rotation_in_degrees)`
  - [ ] Stocker le résultat dans une variable temporaire

- [ ] **Étape 2 : Application de la translation**
  - [ ] Appeler `apply_translation(rotated_minutia, row_translation, column_translation)`
  - [ ] Stocker le résultat final

- [ ] **Retour**
  - [ ] Retourner la minutie doublement transformée

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_apply_transformation_single()` → "OK"

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 3.4. Fonction `apply_transformation` (surcharge liste)

**Signature :**
```cpp
std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae,
                                         int center_row,
                                         int center_column,
                                         int row_translation,
                                         int column_translation,
                                         int rotation_in_degrees);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Même branche que précédemment
  - [ ] Comprendre : appliquer la transformation à chaque minutie
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Créer un vecteur vide : `std::vector<Minutia> transformed;`

- [ ] **Parcours de toutes les minuties**
  - [ ] Boucle `for` sur chaque minutie du vecteur d'entrée

- [ ] **Transformation individuelle**
  - [ ] Pour chaque minutie : appeler la version simple de `apply_transformation()`
  - [ ] Ajouter le résultat au vecteur avec `push_back()`

- [ ] **Retour**
  - [ ] Retourner le vecteur complet des minuties transformées

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_apply_transformation_list()` → "OK"

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 3.5. Fonction `matching_minutiae_count`

**Signature :**
```cpp
unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1,
                                     const std::vector<Minutia> &minutiae_2,
                                     unsigned int max_distance,
                                     unsigned int max_orientation);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/matching-count`
  - [ ] Relire la section 3.8 du PDF
  - [ ] Comprendre : comptage avec seuils de tolérance
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Initialisation**
  - [ ] Initialiser `count = 0`

- [ ] **Double boucle sur les minuties**
  - [ ] Boucle externe : pour chaque minutie m1 de la liste 1
  - [ ] Boucle interne : pour chaque minutie m2 de la liste 2

- [ ] **Calcul de la distance euclidienne**
  - [ ] Formule : `sqrt((m1.row - m2.row)² + (m1.column - m2.column)²)`
  - [ ] Utiliser `std::sqrt()` et élever au carré avec `* val`
  - [ ] Stocker dans une variable `distance`

- [ ] **Calcul de la différence d'orientation**
  - [ ] Formule : `|m1.orientation - m2.orientation|`
  - [ ] Utiliser `std::abs()`
  - [ ] Stocker dans une variable `diff_orientation`

- [ ] **Test des seuils**
  - [ ] Si `distance <= max_distance` ET `diff_orientation <= max_orientation` :
    - [ ] Incrémenter `count`
    - [ ] **Sortir de la boucle interne** (break)
    - [ ] (Une minutie de la liste 1 ne peut matcher qu'une seule minutie de la liste 2)

- [ ] **Retour**
  - [ ] Retourner `count` (nombre total de minuties correspondantes)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `test_matching_minutiae_count_basic()` → "OK"
  - [ ] Test manuel : deux listes identiques → count = taille de la liste
  - [ ] Test manuel : listes très différentes → count = 0

- [ ] **Finalisation**
  - [ ] Commit et push
  - [ ] Pull Request + review

---

#### 3.6. Fonction `match` (FONCTION FINALE)

**Signature :**
```cpp
bool match(const std::vector<Minutia> &minutiae_1,
          const std::vector<Minutia> &minutiae_2);
```

**Checklist d'implémentation :**

- [ ] **Préparation**
  - [ ] Créer branche : `git checkout -b feature/match`
  - [ ] Relire la section 3.9 du PDF
  - [ ] Comprendre : force brute sur toutes les paires + rotations
  - [ ] Ouvrir la fonction
  - [ ] Supprimer `NotImplemented();`

- [ ] **Stratégie générale**
  - [ ] Pour chaque paire (m1, m2) de minuties des deux listes :
    - [ ] Tenter de superposer m2 sur m1
    - [ ] Tester plusieurs rotations autour de la rotation de base
    - [ ] Si ≥ FOUND_THRESHOLD minuties matchent → retourner true immédiatement

- [ ] **Double boucle sur les paires de minuties**
  - [ ] Boucle externe : pour chaque m1 dans minutiae_1
  - [ ] Boucle interne : pour chaque m2 dans minutiae_2

- [ ] **Calcul des paramètres de transformation**
  - [ ] Centre de rotation : `center_row = m1.row`, `center_col = m1.column`
  - [ ] Translation verticale : `row_trans = m2.row - m1.row`
  - [ ] Translation horizontale : `col_trans = m2.column - m1.column`
  - [ ] Rotation de base : `base_rotation = m2.orientation - m1.orientation`

- [ ] **Boucle sur la plage de rotations**
  - [ ] De `-MATCH_ANGLE_OFFSET` à `+MATCH_ANGLE_OFFSET` (inclus)
  - [ ] Pour chaque `angle_offset` :
    - [ ] Calculer : `rotation = base_rotation + angle_offset`

- [ ] **Application de la transformation**
  - [ ] Appeler `apply_transformation(minutiae_2, center_row, center_col, row_trans, col_trans, rotation)`
  - [ ] Stocker le résultat dans un vecteur temporaire

- [ ] **Comptage des correspondances**
  - [ ] Appeler `matching_minutiae_count(minutiae_1, transformed, DISTANCE_THRESHOLD, ORIENTATION_THRESHOLD)`
  - [ ] ⚠️ Utiliser les constantes définies dans le .hpp

- [ ] **Test du seuil**
  - [ ] Si `matches >= FOUND_THRESHOLD` :
    - [ ] **Retourner true immédiatement** (match trouvé, pas besoin de continuer)

- [ ] **Aucun match trouvé**
  - [ ] Si toutes les itérations sont terminées sans avoir retourné true :
    - [ ] Retourner false (empreintes différentes)

- [ ] **Tests**
  - [ ] Compiler
  - [ ] Exécuter `compare fingerprints` → tous les tests "OK"
  - [ ] Vérifier :
    - [ ] 1_1 vs 1_2 → true (même doigt)
    - [ ] 1_1 vs 1_3 → true (même doigt)
    - [ ] 1_1 vs 2_1 → false (doigts différents)
  - [ ] Exécuter `test_match_succeeds_with_translation()` → "OK"
  - [ ] Exécuter `test_match_insufficient_minutiae()` → "OK"

- [ ] **Finalisation Partie 3**
  - [ ] Commit et push
  - [ ] Pull Request finale
  - [ ] Review exhaustive avec binôme (fonction critique)
  - [ ] Merge dans main après validation
  - [ ] 🎉 **PROJET COMPLET !**

---

### 🏁 Phase Finale : Tests et Validation Complète

#### F.1. Tests Automatiques Complets

- [ ] **Exécution mode verbeux**
  - [ ] Lancer le programme avec l'option `-v`
  - [ ] Vérifier que TOUS les tests affichent "OK"

- [ ] **Test Partie 1**
  - [ ] Exécuter `test_part1()` → tous OK
  - [ ] Vérifier les outputs :
    - [ ] `skeleton_1_1.png`
    - [ ] `skeleton_1_2.png`
    - [ ] `skeleton_2_1.png`

- [ ] **Test Partie 2**
  - [ ] Exécuter `test_part2()` → tous OK
  - [ ] Vérifier les outputs :
    - [ ] `minutiae_1_1.png`
    - [ ] `minutiae_1_2.png`
    - [ ] `minutiae_2_1.png`

- [ ] **Test Partie 3**
  - [ ] Exécuter `test_part3()` → tous OK
  - [ ] Vérifier toutes les comparaisons d'empreintes

#### F.2. Validation Visuelle

- [ ] **Squelettes**
  - [ ] Comparer chaque skeleton_*.png avec les références
  - [ ] Vérifier : traits de 1 pixel d'épaisseur
  - [ ] Vérifier : structure préservée (pas de coupures)
  - [ ] Vérifier : absence de pixels rouges (pixels en trop)

- [ ] **Minuties**
  - [ ] Ouvrir chaque minutiae_*.png
  - [ ] Vérifier : tous les points d'intérêt marqués
  - [ ] Vérifier : aucun faux positif (cercle rouge sur un non-point)
  - [ ] Vérifier : orientations cohérentes (flèches pointent dans le bon sens)

#### F.3. Tests de Régression

- [ ] **Relancer tous les tests après chaque merge**
- [ ] **Documenter tout comportement inattendu**
- [ ] **Fixer les bugs avant de continuer**

#### F.4. Revue de Code Finale

- [ ] **Code propre**
  - [ ] Pas de code commenté inutile
  - [ ] Pas de NotImplemented() restant
  - [ ] Noms de variables explicites
  - [ ] Indentation cohérente

- [ ] **Commentaires**
  - [ ] Commentaires pour les parties complexes
  - [ ] Explication des algorithmes non triviaux
  - [ ] Pas de sur-commentaire (code auto-explicatif)

- [ ] **Respect des consignes**
  - [ ] Seul `fingerprint.cpp` a été modifié
  - [ ] Pas de modification des .hpp fournis
  - [ ] Toutes les fonctions implémentées

#### F.5. Documentation

- [ ] **README.md à jour**
  - [ ] Toutes les checkboxes cochées
  - [ ] Notes sur problèmes rencontrés et solutions
  - [ ] Statistiques : temps passé, nombre de commits, etc.

- [ ] **docs/notes_techniques.md**
  - [ ] Décisions d'implémentation importantes
  - [ ] Optimisations effectuées
  - [ ] Limites connues

- [ ] **docs/bugs_connus.md**
  - [ ] Liste des bugs identifiés mais non critiques
  - [ ] Workarounds utilisés

---

## 🔧 Méthodologie de Travail

### Principes de Développement

#### 1. Test-Driven Development (TDD) Adapté
```
┌─────────────────┐
│ 1. Lire spec    │
│    fonction     │
└────────┬────────┘
         │
    ┌────▼─────────┐
    │ 2. Implémenter│
    │    fonction   │
    └────┬─────────┘
         │
    ┌────▼────────┐
    │ 3. Compiler │
    └────┬────────┘
         │
    ┌────▼──────────┐
    │ 4. Tester     │
    │    unitaire   │
    └────┬──────────┘
         │
    ┌────▼────────┐
    │ 5. Debug si  │
    │    nécessaire│
    └────┬─────────┘
         │
    ┌────▼────────┐
    │ 6. Commit   │
    └─────────────┘
```

#### 2. Développement Incrémental

**Ne jamais implémenter plusieurs fonctions à la fois !**

✅ **BON** :
1. Implémenter `get_neighbours`
2. Tester
3. Commit
4. Passer à `black_neighbours`

❌ **MAUVAIS** :
1. Implémenter `get_neighbours`, `black_neighbours`, `transitions` d'un coup
2. Tout tester en même temps
3. Debugger un gros bloc de code

#### 3. Gestion des Erreurs
```cpp
// Toujours vérifier les cas limites
if (invalid_input) {
    throw std::invalid_argument("Message explicite");
}

// Utiliser les assertions pour le debug
assert(condition && "Message d'erreur");
```

#### 4. Debugging Efficace

**Outils à utiliser :**

| Outil | Utilisation | Quand |
|-------|-------------|-------|
| **Debugger Qt** | Points d'arrêt, step-by-step | Bugs logiques |
| **Print statements** | Afficher valeurs intermédiaires | Debug rapide |
| **Fonctions de visualisation** | `draw_minutia()`, `add_circle()` | Vérification visuelle |
| **Tests unitaires** | Valider chaque fonction | Après implémentation |

**Stratégie de debug :**

1. **Isoler le problème** : Quelle fonction échoue ?
2. **Reproduire** : Test minimal qui expose le bug
3. **Hypothèse** : Qu'est-ce qui pourrait causer ça ?
4. **Vérification** : Tester l'hypothèse
5. **Fix** : Corriger
6. **Régression** : Relancer TOUS les tests

### Gestion du Temps

#### Checkpoints Hebdomadaires

**Fin Semaine 1 :**
- [ ] Partie 1 complète et validée
- [ ] Tous les tests de squelettisation passent
- [ ] Images squelettes visuellement correctes

**Fin Semaine 2 :**
- [ ] Partie 2 complète et validée
- [ ] Extraction de minuties fonctionnelle
- [ ] Orientations calculées correctement

**Fin Semaine 3 :**
- [ ] Partie 3 complète et validée
- [ ] Matching fonctionnel
- [ ] Projet finalisé et documenté

---

## 💻 Installation et Configuration

### Prérequis Système
```bash
# Vérifier version C++
g++ --version  # Minimum : GCC 7+ / Clang 5+ / MSVC 2017+

# Vérifier C++11 supporté
g++ -std=c++11 --version
```

**Outils requis :**
- Qt Creator 4.x+ (IDE recommandé)
- Compilateur C++11 minimum
- Git 2.x+
- CMake 3.10+ (si build système utilisé)

### Installation Qt Creator

#### 🐧 Linux (Debian/Ubuntu)
```bash
sudo apt update
sudo apt install qtcreator build-essential
sudo apt install qt5-default  # Qt5 libraries
```

#### 🍎 macOS
```bash
# Via Homebrew
brew install qt
brew install qt-creator

# Ou télécharger depuis qt.io
```

### Configuration du Projet

#### Méthode 1 : Qt Creator (Recommandée)
```
1. Lancer Qt Creator
2. File → Open File or Project
3. Naviguer vers le dossier du projet
4. Sélectionner le fichier .pro (ou CMakeLists.txt)
5. Configurer le kit de compilation
   - Desktop Qt [version] (Kit par défaut)
6. Build → Build Project (Ctrl+B / Cmd+B)
7. Run → Run (Ctrl+R / Cmd+R)
```

#### Méthode 2 : Ligne de Commande
```bash
# Cloner/extraire le projet
cd projet-empreintes/

# Compilation manuelle
g++ -std=c++11 -o fingerprint \
    src/fingerprint.cpp \
    src/helper.cpp \
    src/main.cpp \
    -I./src \
    -lm  # Bibliothèque mathématique

# Exécution
./fingerprint

# Exécution en mode verbeux
./fingerprint -v
```

### Configuration des Arguments de Ligne de Commande

**Dans Qt Creator :**
```
1. Projects (panneau gauche)
2. Build & Run → Run
3. Command line arguments : -v
4. Apply
```

**Effet :** Active le mode verbeux pour les tests

### Structure des Dossiers de Travail
```bash
# Créer la structure recommandée
mkdir -p docs
touch docs/notes_techniques.md
touch docs/decisions.md
touch docs/bugs_connus.md
```

---

## 🧪 Tests et Validation

### Framework de Tests Fourni

Le projet inclut un système de tests dans `main.cpp` :
```cpp
// Fonctions de test disponibles
test_part1();  // Tests Partie 1 (Squelettisation)
test_part2();  // Tests Partie 2 (Extraction)
test_part3();  // Tests Partie 3 (Comparaison)
```

### Exécution des Tests

#### Mode Normal
```bash
./fingerprint
```

Affiche uniquement les résultats : `OK` ou `FAILED`

#### Mode Verbeux
```bash
./fingerprint -v
```

Affiche :
- Données de test utilisées
- Résultats attendus vs résultats obtenus
- Détails des échecs

### Types de Tests

#### 1. Tests Unitaires

Testent une fonction isolée :
```
test_get_neighbours_1()
test_get_neighbours_2()
test_black_neighbours()
test_transitions()
...
```

**Interprétation :**
- ✅ `OK` : Fonction correcte pour ce cas
- ❌ `FAILED` : Erreur détectée
  - Compare `Expected` vs `Actual`
  - Identifie la différence

#### 2. Tests d'Intégration

Testent plusieurs fonctions ensemble :
```
test_thin()              # Utilise get_neighbours, transitions, etc.
test_extract_single_minutia()  # Utilise toutes les fonctions de Partie 2
```

#### 3. Tests Visuels

Génèrent des images à comparer manuellement :
```
test_draw_skeleton()     # Génère skeleton_*.png
draw_minutiae()          # Génère minutiae_*.png
```

**Processus de validation :**
1. Ouvrir l'image générée
2. Ouvrir l'image de référence (`resources/test_outputs/`)
3. Comparer visuellement
4. En cas d'erreur : pixels différents marqués en rouge

#### 4. Tests de Comparaison

Testent le matching complet :
```
compare fingerprints     # Teste toutes les paires d'empreintes
```

**Matrice de tests :**

| Comparaison | Attendu | Description |
|-------------|---------|-------------|
| 1_1 vs 1_2 | MATCH | Même doigt, position différente |
| 1_1 vs 1_3 | MATCH | Même doigt, rotation |
| 1_1 vs 2_1 | NO MATCH | Doigts différents |
| ... | ... | ... |

### Debugging des Tests Échoués

#### Étape 1 : Identifier la Source
```bash
# Exemple de sortie FAILED
test_get_neighbours_1
FAILED
Expected: {false, false, false, false, false, false, false, false}
Actual:   {false, true, false, false, false, false, false, false}
```

**Analyse :**
- La fonction retourne un voisin à `true` alors qu'il devrait être `false`
- C'est le voisin P1 (index 1) qui est incorrect

#### Étape 2 : Placer des Points d'Arrêt
```cpp
// Dans Qt Creator
1. Ouvrir fingerprint.cpp
2. Clic gauche dans la marge → point d'arrêt rouge
3. Debug → Start Debugging (F5)
4. Examiner les variables
```

#### Étape 3 : Vérifier les Hypothèses
```cpp
// Ajouter des prints temporaires
std::cout << "row: " << row << ", col: " << col << std::endl;
std::cout << "neighbour P1: " << neighbours[1] << std::endl;
```

#### Étape 4 : Corriger et Retester
```bash
# Recompiler
# Relancer le test spécifique
# Vérifier que tous les tests passent (pas juste celui-ci)
```

### Création de Tests Personnalisés
```cpp
// Dans main.cpp (modifiable)

void test_custom() {
    // Votre test personnalisé
    BinaryImage test_img = ...;
    auto result = get_neighbours(test_img, 5, 5);
    
    // Vérification manuelle
    if (result.size() == 8) {
        std::cout << "Custom test: OK" << std::endl;
    } else {
        std::cout << "Custom test: FAILED" << std::endl;
    }
}
```

### Couverture de Tests Recommandée

Pour chaque fonction implémentée :

- [ ] **Test nominal** : Cas d'usage standard
- [ ] **Test limite supérieure** : Valeurs max
- [ ] **Test limite inférieure** : Valeurs min
- [ ] **Test cas d'erreur** : Entrées invalides
- [ ] **Test cas vide** : Tableaux vides, etc.

### Validation Finale Avant Rendu
```bash
# Checklist de validation
✓ Tous les tests automatiques passent (mode verbeux)
✓ Toutes les images générées correctes visuellement
✓ Aucun warning de compilation
✓ Aucun code commenté inutile
✓ Aucun NotImplemented() restant
✓ Documentation à jour
✓ Commit final avec message clair
```

---

## 🌳 Gestion de Version

### Structure des Branches Git
```
main (branche protégée)
├── feature/get-neighbours
├── feature/black-neighbours
├── feature/transitions
├── feature/identical
├── feature/thinning-step
├── feature/thin
├── feature/connected-pixels
├── feature/compute-slope
├── feature/compute-angle
├── feature/compute-orientation
├── feature/extract
├── feature/apply-rotation
├── feature/apply-translation
├── feature/apply-transformation
├── feature/matching-count
└── feature/match
```

### Workflow Git Recommandé

#### 1. Création d'une Branche Feature
```bash
# Toujours partir de main à jour
git checkout main
git pull origin main

# Créer une branche pour la feature
git checkout -b feature/nom-fonction

# Exemple
git checkout -b feature/get-neighbours
```

#### 2. Développement sur la Branche
```bash
# Faire des commits réguliers
git add src/fingerprint.cpp
git commit -m "Add get_neighbours implementation"

# Commits atomiques recommandés
git commit -m "Add parameter validation for get_neighbours"
git commit -m "Handle edge pixels in get_neighbours"
git commit -m "Add unit tests for get_neighbours"
```

#### 3. Push et Pull Request
```bash
# Pousser la branche
git push origin feature/get-neighbours

# Sur GitHub :
# 1. Créer une Pull Request
# 2. Assigner le binôme comme reviewer
# 3. Attendre la review et les commentaires
```

#### 4. Review de Code

**Pour le reviewer :**
```
✓ Code clair et lisible
✓ Respect des conventions
✓ Tests passent
✓ Pas de code dupliqué
✓ Gestion des cas limites
✓ Commentaires si nécessaire
```

**Actions possibles :**
- ✅ **Approve** → Merge autorisé
- 💬 **Comment** → Demande de modifications
- ❌ **Request changes** → Modifications obligatoires

#### 5. Merge dans Main
```bash
# Option 1 : Via GitHub UI (recommandé)
# Bouton "Merge Pull Request"

# Option 2 : En ligne de commande
git checkout main
git merge feature/get-neighbours
git push origin main

# Supprimer la branche feature
git branch -d feature/get-neighbours
git push origin --delete feature/get-neighbours
```

### Conventions de Commits

#### Format Recommandé
```
<type>: <description courte>

<description détaillée optionnelle>

<footer optionnel>
```

#### Types de Commits

| Type | Usage | Exemple |
|------|-------|---------|
| `feat` | Nouvelle fonctionnalité | `feat: implement get_neighbours` |
| `fix` | Correction de bug | `fix: handle edge pixels in get_neighbours` |
| `test` | Ajout/modification de tests | `test: add test cases for transitions` |
| `refactor` | Refactoring (pas de changement fonctionnel) | `refactor: simplify thinning_step logic` |
| `docs` | Documentation | `docs: update README with installation steps` |
| `style` | Formatage, indentation | `style: fix indentation in fingerprint.cpp` |
| `chore` | Maintenance, config | `chore: update .gitignore` |

#### Exemples de Bons Commits
```bash
git commit -m "feat: implement get_neighbours function

- Extract 8 neighbors in P0-P7 order
- Handle edge pixels by returning false
- Validate input coordinates
- Add comprehensive unit tests"

git commit -m "fix: correct y-axis inversion in compute_slope

The coordinate system in images has y-axis pointing down,
while mathematical formulas assume y-axis pointing up.
Fixed by inverting y calculation: y = row_minutia - row_pixel"

git commit -m "test: add edge case tests for black_neighbours

- Test with empty vector
- Test with all black pixels
- Test with all white pixels"
```

### Gestion des Conflits

#### Prévention
```bash
# Toujours travailler sur des branches features
# Communiquer sur qui travaille sur quoi
# Pull régulièrement depuis main
git checkout feature/ma-branche
git fetch origin
git rebase origin/main
```

#### Résolution
```bash
# Si conflit lors du merge
git status  # Voir les fichiers en conflit

# Éditer les fichiers manuellement
# Chercher les marqueurs <<<<<<<, =======, >>>>>>>
# Choisir la version correcte

# Marquer comme résolu
git add fichier_en_conflit.cpp
git commit -m "fix: resolve merge conflict in fingerprint.cpp"
```

### .gitignore Recommandé
```gitignore
# Compiled files
*.o
*.so
*.a
fingerprint

# Qt Creator
*.pro.user
*.pro.user.*
*.autosave

# Build directories
build/
build-*/

# Images générées (résultats de tests)
skeleton_*.png
minutiae_*.png

# Backup files
*~
*.bak
*.swp

# IDE specific
.vscode/
.idea/
*.iml

# OS specific
.DS_Store
Thumbs.db

# Documentation build
docs/_build/
```

### Tags et Releases
```bash
# Créer un tag après chaque partie complétée
git tag -a v1.0-partie1 -m "Partie 1 complète et validée"
git push origin v1.0-partie1

git tag -a v2.0-partie2 -m "Partie 2 complète et validée"
git push origin v2.0-partie2

git tag -a v3.0-final -m "Projet final - toutes parties complètes"
git push origin v3.0-final
```

### Backup et Sécurité
```bash
# Pousser régulièrement vers le remote
git push origin --all

# En cas de problème, revenir à un commit précédent
git log --oneline
git checkout <commit-hash>

# Créer une branche de backup
git branch backup-$(date +%Y%m%d)
git push origin backup-$(date +%Y%m%d)
```

---

## 📚 Ressources et Documentation

### Documentation Officielle

#### Documents Fournis

| Document | Description | À Lire |
|----------|-------------|---------|
| `fingerprints_fr.pdf` | Énoncé complet du projet | ⭐⭐⭐⭐⭐ |
| `fingerprint.hpp` | Prototypes et structures | ⭐⭐⭐⭐⭐ |
| `helper.hpp` | Fonctions utilitaires | ⭐⭐⭐ |
| Annexe A.1 (PDF) | Conversions size_t/int | ⭐⭐⭐⭐ |
| Annexe A.2 (PDF) | Régression linéaire | ⭐⭐⭐ |
| Annexe A.3 (PDF) | Sorties attendues des tests | ⭐⭐⭐⭐ |

### Références Techniques C++

#### Structures de Données
```cpp
// Vecteurs 2D
std::vector<std::vector<bool>> image;
image.size()           // Nombre de lignes
image[0].size()        // Nombre de colonnes
image[row][col]        // Accès à un pixel

// Vecteurs 1D
std::vector<Minutia> minutiae;
minutiae.push_back(m)  // Ajouter élément
minutiae.size()        // Nombre d'éléments
```

#### Fonctions Mathématiques
```cpp
#include <cmath>

std::sqrt(x)           // Racine carrée
std::pow(x, 2)         // Puissance (ou x*x)
std::abs(x)            // Valeur absolue
std::cos(theta)        // Cosinus (theta en radians)
std::sin(theta)        // Sinus (theta en radians)
std::atan(slope)       // Arc tangente
std::round(x)          // Arrondi
```

#### Conversions
```cpp
// Degrés ↔ Radians (fonctions fournies)
radians_to_degrees(angle)
degrees_to_radians(angle)

// size_t ↔ int (fonctions fournies)
to_int(size_t_value)
to_size_t(int_value)

// Type casting
static_cast<int>(double_value)
```

### Concepts Algorithmiques

#### 1. Squelettisation (Thinning)

**Algorithme itératif** qui supprime progressivement les pixels non essentiels.

**Références :**
- Zhang-Suen Thinning Algorithm
- Morphological Skeletonization

**Complexité :** O(n × m × k) où k = nombre d'itérations

#### 2. Régression Linéaire

**But :** Trouver la droite qui passe au plus près d'un ensemble de points.

**Formule :** y = ax + b (ici b=0 car origine = minutie)

**Méthode :** Moindres carrés (minimiser l'erreur quadratique)

#### 3. Matching par Force Brute

**Stratégie :** Tester toutes les combinaisons possibles.

**Optimisations possibles (non requises) :**
- Trier les minuties par position
- Utiliser un octree ou k-d tree
- RANSAC pour robustesse

**Complexité :** O(n² × m × k) où n = minuties_1, m = minuties_2, k = angles testés

### Ressources en Ligne

#### C++ et Algorithmique

- [cppreference.com](https://en.cppreference.com/) - Référence C++ complète
- [cplusplus.com](http://www.cplusplus.com/) - Tutoriels et référence
- [Stack Overflow](https://stackoverflow.com/questions/tagged/c%2b%2b) - Forum Q&A

#### Biométrie et Empreintes Digitales

- [NIST Biometric Standards](https://www.nist.gov/programs-projects/biometrics)
- [Fingerprint Recognition Overview](https://en.wikipedia.org/wiki/Fingerprint_recognition)

#### Git et Collaboration

- [Pro Git Book](https://git-scm.com/book/en/v2) - Guide complet Git
- [GitHub Flow](https://guides.github.com/introduction/flow/) - Workflow recommandé
- [Conventional Commits](https://www.conventionalcommits.org/) - Convention de commits

### Outils de Debug

#### 1. Qt Creator Debugger
```
Utilisation :
1. Placer un point d'arrêt (clic dans marge)
2. Debug → Start Debugging (F5)
3. Utiliser les boutons :
   - Continue (F5)
   - Step Over (F10)
   - Step Into (F11)
   - Step Out (Shift+F11)
4. Examiner les variables dans le panneau
```

#### 2. GDB (Ligne de Commande)
```bash
# Compiler avec symboles de debug
g++ -std=c++11 -g -o fingerprint src/*.cpp

# Lancer GDB
gdb ./fingerprint

# Commandes utiles
(gdb) break fingerprint.cpp:42  # Point d'arrêt
(gdb) run                       # Exécuter
(gdb) print variable            # Afficher variable
(gdb) step                      # Ligne par ligne
(gdb) continue                  # Continuer
(gdb) backtrace                 # Stack trace
```

#### 3. Valgrind (Détection de Fuites Mémoire)
```bash
# Installer
sudo apt install valgrind  # Linux

# Exécuter
valgrind --leak-check=full ./fingerprint
```

### Antisèches (Cheat Sheets)

#### Commandes Git Essentielles
```bash
# Configuration
git config --global user.name "Votre Nom"
git config --global user.email "email@example.com"

# Workflow de base
git status                      # État du repo
git add fichier.cpp            # Ajouter au staging
git commit -m "message"        # Créer un commit
git push origin nom-branche    # Pousser vers remote
git pull origin main           # Récupérer les changements

# Branches
git branch                     # Lister les branches
git checkout -b nouvelle       # Créer et basculer
git checkout nom-branche       # Basculer vers branche
git merge autre-branche        # Fusionner une branche

# Historique
git log --oneline              # Historique compact
git diff                       # Voir les changements
git show <commit-hash>         # Détails d'un commit

# Annulation
git reset HEAD fichier         # Unstage un fichier
git checkout -- fichier        # Annuler modifications
git revert <commit-hash>       # Annuler un commit
```

#### Syntaxe C++ Essentielle
```cpp
// Boucles
for (size_t i = 0; i < size; ++i) { }
for (const auto& elem : vector) { }
while (condition) { }

// Conditions
if (condition) { } else { }
condition ? valeur_si_vrai : valeur_si_faux;

// Fonctions
return_type function_name(params) {
    // corps
    return value;
}

// Vecteurs
std::vector<type> vec;
vec.push_back(element);
vec.size();
vec[index];
vec.empty();

// Structures
struct Name {
    type1 field1;
    type2 field2;
};
Name instance;
instance.field1 = value;
```

### FAQ Technique

#### Q : "Mon code compile mais les tests échouent, pourquoi ?"

**R :** Vérifiez :
1. Les indices de tableau (0-indexed)
2. Les conversions size_t ↔ int
3. L'ordre des voisins P0-P7
4. L'inversion de l'axe Y dans le système de coordonnées

#### Q : "J'ai un segmentation fault, comment débugger ?"

**R :**
1. Compiler avec `-g` (symboles debug)
2. Lancer avec gdb
3. Vérifier les accès hors limites de tableaux
4. Vérifier les pointeurs null

#### Q : "Comment vérifier visuellement mes résultats ?"

**R :**
1. Utiliser les fonctions de visualisation fournies
2. Comparer avec les images de référence
3. Ouvrir les images dans Qt Creator ou un viewer

#### Q : "Quelle est la différence entre step 0 et step 1 ?"

**R :** Seules les conditions 5 et 6 changent :
- Step 0 : P0|P2|P4 blanc ET P2|P4|P6 blanc
- Step 1 : P0|P2|P6 blanc ET P0|P4|P6 blanc

#### Q : "Pourquoi utiliser size_t au lieu de int ?"

**R :** size_t est le type standard pour les tailles et indices en C++. Il est non signé et garantit de pouvoir représenter n'importe quelle taille valide.

#### Q : "Comment gérer les conversions entre degrés et radians ?"

**R :** Utiliser les fonctions fournies :
- `degrees_to_radians()` avant std::cos/sin
- `radians_to_degrees()` après std::atan

---

## 📝 Templates Utiles

### Template de Commit
```
<type>: <description courte (50 chars max)>

<description détaillée>
- Changement 1
- Changement 2
- Changement 3

Fixes #<numéro issue si applicable>
```

### Template de Pull Request
```markdown
## Description

[Description courte de ce qui a été implémenté]

## Fonction(s) implémentée(s)

- [ ] `function_name1`
- [ ] `function_name2`

## Tests

- [x] Tests unitaires passent
- [x] Tests d'intégration passent
- [x] Validation visuelle effectuée

## Checklist

- [x] Code compilé sans warnings
- [x] Conventions de nommage respectées
- [x] Commentaires ajoutés si nécessaire
- [x] Documentation à jour

## Notes

[Toute information supplémentaire pertinente]
```

### Template de Bug Report
```markdown
## Description du Bug

[Description claire et concise]

## Comment Reproduire

1. Étape 1
2. Étape 2
3. Étape 3

## Comportement Attendu

[Ce qui devrait se passer]

## Comportement Observé

[Ce qui se passe réellement]

## Contexte

- Fonction concernée : [nom]
- Fichier : [nom]
- Ligne : [numéro]
- Système : [OS + version]
- Compilateur : [version]

## Logs / Erreurs
```
[Copier-coller les messages d'erreur]
```

## Tentatives de Solution

[Ce que vous avez déjà essayé]
```

---

## 🎓 Auteurs et Crédits

**Projet Académique**  
École Polytechnique Fédérale de Lausanne (EPFL)  
Cours ICC (SV) - Informatique Calcul et Communication 
Version 1.1

**Encadrants :**
- Barbara Jobstmann
- Jamila Sam
- Rafael Pires
- Amir Bouchaoui

**Étudiants :**
- [Déglon Louis]
- [Caudron Eulalie]

---

## 📊 Métriques du Projet

### Statistiques de Développement
```markdown
📅 Date de Début : [24/11/2025]
📅 Date de Fin : [17/12/2025]
```

### Remerciements
```markdown
Merci à claude pour le markdown, big up a mon cousin du bled et a markus le chien jaune. Bisous maman !
```

---

## ⚖️ Licence et Intégrité Académique

> **⚠️ IMPORTANT : Ce projet est soumis aux règles d'intégrité académique de l'EPFL.**
