# Jeu de Rétrogradation - Résolveur

## Vue d'ensemble
Ce programme résout des **jeux combinatoires** sur un graphe orienté en utilisant l'algorithme de **rétrogradation (backward induction)**. Il détermine si un joueur peut gagner, perdre ou être en situation de nulle (boucle infinie) à partir d'une position de départ.

---

## Concept du Jeu

- **Deux joueurs** alternatifs jouent sur les sommets d'un graphe
- À chaque tour, un joueur se déplace vers un sommet exactement **k arcs** plus loin
- Le joueur qui commence son tour sur le sommet **"arrivée"** **perd**
- Le jeu s'arrête quand un joueur ne peut pas faire k pas (sommet sans issue)

---

## Comment fonctionne le programme

### 1. **Lecture des données** (Input)
```
n m k dep arr
u₁ v₁
u₂ v₂
...
```
- `n` : nombre de sommets
- `m` : nombre d'arcs (arêtes dirigées)
- `k` : nombre de pas à effectuer à chaque coup
- `dep` : sommet de départ
- `arr` : sommet d'arrivée (but perdant)

### 2. **Préparation du graphe** (Fonction `preparer()`)
- Pour chaque sommet `i`, calcule **tous les sommets atteignables à distance exacte k**
- Stocke les destinations : `dest_k[i]` = liste des sommets à k arcs de `i`
- Stocke les prédécesseurs : `pred_k[i]` = liste des sommets pouvant atteindre `i` en k coups
- Initialise les compteurs : chaque sommet a un compteur = nombre de ses destinations

### 3. **Rétrogradation** (Fonction `resoudre()`)
L'algorithme classifie chaque sommet en 3 états :

#### **PERDANT (P - rouge)**
- Le sommet d'arrivée (joueur commençant là perd)
- Tous les sommets n'ayant aucune destination à distance k (joueur bloqué)

#### **GAGNANT (G - vert)**
- Sommet d'où on peut atteindre directement un position PERDANT en k pas
- Donc le joueur peut forcer l'adversaire vers une défaite

#### **NUL (N - gris)**
- Sommet coincé dans une boucle infinie d'états INCONNU
- Aucun joueur ne peut forcer la victoire

### 4. **Propagation en queue** (BFS)
```
Tant que la file n'est pas vide :
  - Prendre un sommet v classifié
  - Pour chaque prédécesseur u de v :
    - Si v est PERDANT → u est GAGNANT (peut atteindre perdant)
    - Si v est GAGNANT → décrémenter compteur de u
      - Si compteur de u = 0 → TOUTES destinations sont GAGNANTES → u est PERDANT
```

### 5. **Sortie graphique** (Mermaid.js)
Génère un diagramme du graphe avec couleurs :
- 🟢 GAGNANT (victoire possible)
- 🔴 PERDANT (défaite assurée)
- ⚪ NUL (boucle/pas de résolution)

Le site de visualisation recommandé est https://mermaid.live/
Le programme sorte un fichier Mermaid.js après l'exécution, l'utilisateur peut exporter et ouvrir le fichier avec une logiciel de visualisation
---

## Exemple
```
Graphe: 0 → 1 → 3 ← 2 ← 0
        └────────┘
k = 1 (un seul pas)
Arrivée = 3

Résultat:
- Sommet 0: GAGNANT (peut atteindre l'arrivée via 1→3)
- Sommet 1: PERDANT (1→3 = défaite)
- Sommet 2: GAGNANT (peut atteindre l'arrivée via 3)
- Sommet 3: PERDANT (arrivée = perte automatique)
```

---

## Complexité

| Opération | Complexité |
|-----------|-----------|
| Construction graph | O(n + m) |
| Calcul destinations k | O(n × arêtes^k) |
| Rétrogradation | O(n + m) |
| **Total** | **O(n × arêtes^k + m)** |

---

## Validation des entrées
✅ n ≥ 1, k ≥ 1  
✅ 0 ≤ dep, arr < n  
✅ 0 ≤ u, v < n pour chaque arc  
✅ m ≥ 0

---

## Résultat final
Affiche :
1. Diagramme Mermaid du graphe colorisé
2. Verdict au départ : **VICTOIRE** | **DÉFAITE** | **NUL**
