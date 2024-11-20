# Projet-POO-A2-CESI 🧩

Un projet académique en C++ développé dans le cadre du cursus **CESI**.  
Le projet consiste à implémenter une version fonctionnelle du **Jeu de la Vie** en suivant les principes de la programmation orientée objet.

---

## Table des Matières

1. [Description](#description)  
2. [Installation](#installation)  
3. [Structure du Projet](#structure-du-projet)  
4. [Usage](#usage)  
5. [Contribution](#contribution)  
6. [Licence](#licence)

---

## Description

Le **Jeu de la Vie** est un automate cellulaire développé par John Conway. C'est un système où des cellules sur une grille évoluent en fonction de règles simples, produisant des motifs complexes à partir d'initialisations simples.  
Ce projet a été réalisé en binôme pour appliquer les concepts de POO, le développement modulaire, et la gestion collaborative de projets.

---

## Installation

Pour installer et exécuter ce projet, suivez ces étapes :

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/Natjom/Projet-POO-A2-CESI.git
   ```

2. Accédez au répertoire du projet :
   ```bash
   cd Projet-POO-A2-CESI
   ```

3. Compilez le projet à l'aide du Makefile :
   ```bash
   make
   ```

4. Exécutez le programme :
   ```bash
   ./main
   ```

---

## Structure du Projet

Voici la structure des fichiers du projet :

```
Projet-POO-A2-CESI
├── .vscode/          # Configuration de l'environnement VS Code
├── Makefile          # Script de compilation
├── cell.cpp          # Implémentation des comportements des cellules
├── cell.h            # Définition de la classe Cell
├── main.cpp          # Point d'entrée du programme
├── map.cpp           # Implémentation de la gestion de la grille
├── map.h             # Définition de la classe Map
```

**Versions des fichiers :**
- `.vscode` : v1.1811.2 (il y a 2 jours)  
- `Makefile` : Initialisation (il y a 2 jours)  
- `cell.cpp` : v1.2011.1b (il y a 1 minute)  
- `cell.h` : v1.2011.1b (il y a 1 minute)  
- `main.cpp` : v1.2011.1b (il y a 1 minute)  
- `map.cpp` : v1.2011.1b (il y a 1 minute)  
- `map.h` : v1.2011.1b (il y a 1 minute)  

---

## Usage

Après avoir compilé et exécuté le programme, le Jeu de la Vie sera initialisé avec une grille par défaut ou une configuration choisie.  
Suivez les instructions affichées pour :  

- **Démarrer** une simulation.  
- **Modifier** les paramètres (taille de la grille, configuration initiale, etc.).  
- **Observer** l'évolution des cellules selon les règles prédéfinies.  

---

## Contribution

Les contributions sont les bienvenues ! Voici comment contribuer :  

1. Forkez ce dépôt.  
2. Créez une branche pour vos modifications :  
   ```bash
   git checkout -b feature/nom-de-votre-fonctionnalite
   ```  
3. Effectuez vos modifications et ajoutez-les au dépôt :  
   ```bash
   git add .  
   git commit -m "Ajout de fonctionnalité : nom"  
   ```  
4. Poussez vos modifications :  
   ```bash
   git push origin feature/nom-de-votre-fonctionnalite
   ```  
5. Soumettez une Pull Request.  

---

## Licence

Ce projet est sous licence **MIT**.  
Consultez le fichier [LICENSE](LICENSE) pour plus d'informations.  

---

**Lien vers le projet GitHub :**  
[https://github.com/Natjom/Projet-POO-A2-CESI](https://github.com/Natjom/Projet-POO-A2-CESI)
