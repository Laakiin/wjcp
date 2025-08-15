# wjcp

**Outil de traitement et de parsing de données géographiques et API en C**

Ce projet permet de :
- **Parser et traiter des données** (probablement au format JSON).
- **Interagir avec des APIs** ou des données géographiques.
- **Générer une sortie formatée en JSON**.

---
## Structure du projet
- `api.c` / `api.h` : Gestion des appels ou des données API.
- `geo.c` / `geo.h` : Traitement des données géographiques.
- `parser.c` / `parser.h` : Parsing des données d'entrée.
- `main.c` : Point d'entrée du programme.

---
## Langages et outils
- **Langage principal** : C
- **Build** : Makefile

---
## Compilation
Pour compiler le projet, utilisez la commande :
```bash
make
```
*(Assurez-vous d'avoir `gcc` et `make` installés.)*
