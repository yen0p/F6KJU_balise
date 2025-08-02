# Balise Morse HF Multi-bandes F6KJU

Ce projet contient le code source pour une balise Morse multi-bandes (80m à 6m) construite autour d'une carte **Arduino Mega 2560**. La balise est conçue pour être précise et autonome grâce à une synchronisation temporelle et un calcul de position par GPS.

## 🧭 Fonctionnalités

* **Multi-bandes** : Gère les émissions sur 9 bandes radio-amateurs, du 80m au 6m.
* **Synchronisation GPS** : Les émissions sont calées précisément au début de chaque minute grâce aux données d'un module GPS.
* **Locator Dynamique** : Calcule automatiquement le locator Maidenhead sur 6 caractères et peut l'insérer dans le message Morse.
* **Conception Orientée Objet** : Le code est structuré en classes logiques (`GestionnaireGPS`, `GestionnaireDDS`, `EmetteurMorse`) pour une meilleure lisibilité et maintenance.
* **Haute Personnalisation** : L'indicatif, les messages, les fréquences et les broches de contrôle sont facilement configurables via des constantes.
* **Documentation Intégrée** : Le code est entièrement commenté en français selon les standards Doxygen pour une génération facile de la documentation.

---

## 🛠️ Matériel Requis

* Un **Arduino Mega 2560** (ou compatible).
* Un **Générateur de fréquence DDS AD9850**.
* Un **Module GPS** avec sortie série (protocole NMEA, 9600 bauds).
* Une **carte de filtres passe-bas** commutable par des niveaux logiques.
* Un **système d'amplification HF** (préampli + ampli) avec une entrée PTT (Push-To-Talk).
* Une LED et une résistance (220Ω) pour l'indicateur de statut GPS.

---

## 🔌 Schéma de Câblage

| Composant          | Broche du Composant | Broche de l'Arduino Mega 2560 |
| :----------------- | :------------------ | :---------------------------- |
| **Module GPS** | TX (Transmit)       | **19 (RX1)** |
|                    | VCC                 | 5V                            |
|                    | GND                 | GND                           |
| **LED Statut GPS** | Anode (+)           | **22** (via résistance 220Ω)  |
|                    | Cathode (-)         | GND                           |
| **Commande PTT** | Signal PTT          | **24** |
| **DDS AD9850** | DATA                | **30** |
|                    | W_CLK               | **26** |
|                    | FQ_UD               | **28** |
|                    | RESET               | **32** |
| **Filtre 80m** | Commande            | **34** |
| **... (etc)** | ...                 | **...** |

---

## 🚀 Installation

1.  **Prérequis Logiciels** :
    * Installez l'[Arduino IDE](https://www.arduino.cc/en/software).
    * Dans l'Arduino IDE, allez dans `Outils` > `Gérer les bibliothèques...` et installez la bibliothèque **`TinyGPS++`** par Mikal Hart.

2.  **Configuration du Projet** :
    * Ouvrez le fichier `balise_hf.ino`.
    * Vérifiez que les numéros de `BROCHE_...` correspondent à votre câblage.
    * Modifiez le tableau `balises[]` pour ajuster les fréquences ou les messages si nécessaire.

3.  **Téléversement** :
    * Connectez votre Arduino Mega 2560 à votre ordinateur.
    * Dans l'IDE, sélectionnez le bon type de carte (`Outils` > `Type de carte` > `Arduino Mega or Mega 2560`).
    * Sélectionnez le bon port (`Outils` > `Port`).
    * Cliquez sur le bouton "Téléverser".

---

## 📄 Génération de la Documentation

Ce projet est prêt pour [Doxygen](https://www.doxygen.nl/). Pour générer la documentation HTML du code source :

1.  **Installez Doxygen** et (optionnellement) **Graphviz** pour les diagrammes.
2.  Ouvrez un terminal à la racine du projet.
3.  Exécutez la commande :
    ```sh
    doxygen Doxyfile
    ```
4.  Ouvrez le fichier `index.html` situé dans le dossier `doxygen_html` nouvellement créé.

---

## 📂 Structure du Projet

* `balise_hf.ino` : Fichier principal qui contient la logique de `setup()` et `loop()`.
* `GestionnaireGPS.h/.cpp` : Classe gérant la communication avec le module GPS.
* `GestionnaireDDS.h/.cpp` : Classe gérant le contrôle du générateur de fréquence AD9850.
* `EmetteurMorse.h/.cpp` : Classe chargée de la conversion texte->morse et de l'émission.
* `Balise.h/.cpp` : Classe simple contenant les paramètres d'une balise (fréquence, message...).
* `Doxyfile` : Fichier de configuration pour la génération de la documentation.

---

## ⚖️ Licence

Ce projet est distribué sous la **Licence MIT**. Voir le fichier `LICENSE` pour plus de détails.