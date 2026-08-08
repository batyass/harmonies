# 🎨 Harmonies

Implémentation en **C++/Qt** du jeu de plateau *Harmonies* : les joueurs placent des tuiles de paysage, installent des animaux et des esprits de la nature, puis marquent des points en réalisant les motifs demandés par leurs cartes.

Le projet propose **deux interfaces** :

- une **version console** (Terminal) ;
- une **application graphique Qt** avec fenêtre de configuration de la partie.

## Fonctionnalités

- Plateau hexagonal (convention de coordonnées *even-q offset*)
- Règles complètes : placement des tuiles, jetons d'animaux, esprits de la nature, pile de jetons
- Calcul des scores par type de carte (animaux, paysages, esprits de la nature)
- Mode solo avec évaluation dédiée
- Tests unitaires automatisés (`make test`)

## Architecture

Le code source est organisé dans `src/` :

| Dossier      | Rôle                                                        |
| ------------ | ----------------------------------------------------------- |
| `src/core`   | Modèle du jeu : cartes, plateau, jetons, patterns, tours     |
| `src/rules`  | Validation du placement, fin de partie, correspondance de motifs |
| `src/scoring`| Calcul des scores (animaux, paysages, esprits de la nature) |
| `src/qt`     | Interface graphique Qt                                       |
| `src/ui`     | Interface console                                            |
| `src/utils`  | Utilitaires communs (coordonnées hexagonales, etc.)          |

## Compilation & exécution

### Version console

```bash
make run     # compile et lance la partie
make test    # compile et exécute les tests
make clean   # supprime les exécutables
```

### Interface Qt

Ouvrez `HarmoniesQt/HarmoniesQt.pro` dans Qt Creator, ou compilez en ligne de commande :

```bash
cd HarmoniesQt
qmake && make
```

## Convention des coordonnées hexagonales

Le projet utilise une convention de coordonnées **even-q offset** :

- `q` représente la colonne ;
- `r` représente la ligne ;
- les voisins dépendent de la parité de la colonne.

Exemple important : depuis `HexCoord(0,0)`, les voisins sont `(-1,0)`, `(-1,1)`, `(0,1)`, `(1,1)`, `(1,0)`, `(0,-1)`.

Conséquences :

- les patterns des cartes doivent être écrits avec cette convention ;
- les rotations de patterns suivent cette même géométrie ;
- l'affichage Qt et la logique métier utilisent la même convention.

## Documentation

- `docs/notes.md` — notes de développement
- `model.puml`, `diagrammes.puml` — diagrammes UML (PlantUML)
- `Rapport_LO21.pdf` — rapport du projet

## Crédits

Projet réalisé en collaboration avec :

- **Clément Chauvy** — [@Clementcvy](https://github.com/Clementcvy) — dépôt d'origine
- **Neo Gubert** — [@neog21](https://github.com/neog21)
- **Yassir Bizguirne** — [@batyass](https://github.com/batyass)
- ainsi qu'un autre contributeur (`peilin.li@etu.utc.fr`)

Dépôt d'origine : [Clementcvy/LO21projet](https://github.com/Clementcvy/LO21projet)

## Technologies

C++ · Qt · qmake · Makefile · PlantUML
