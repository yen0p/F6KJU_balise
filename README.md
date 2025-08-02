# Balise Morse HF Multi-bandes F6KJU

Ce projet contient le code source pour une balise Morse multi-bandes (80m à 6m) construite autour d'une carte **Arduino Mega 2560**. La balise est conçue pour être précise et autonome grâce à une synchronisation temporelle et un calcul de position par GPS.

## 🧭 Fonctionnalités

* **Multi-bandes** : Gère les émissions sur 9 bandes radio-amateurs, du 80m au 6m.
* **Synchronisation GPS** : Les émissions sont calées précisément au début de chaque minute.
* **Locator Dynamique** : Calcule automatiquement le locator Maidenhead et l'insère dans le message.
* **Indicateurs Visuels** : Une LED dédiée s'allume pour indiquer la bande en cours d'utilisation, et une autre pour signaler l'émission (PTT actif).
* **Conception Orientée Objet** : Le code est structuré en classes logiques pour une meilleure lisibilité et maintenance.
* **Haute Personnalisation** : Indicatif, messages, fréquences et broches sont facilement configurables.
* **Documentation Intégrée** : Le code est entièrement commenté selon les standards Doxygen.

***

## 🛠️ Matériel Requis

* Un **Arduino Mega 2560** (ou compatible).
* Un **Générateur de fréquence DDS AD9850**.
* Un **Module GPS** avec sortie série (protocole NMEA, 9600 bauds).
* Une **carte de filtres passe-bas** commutable.
* Un **système d'amplification HF** (préampli + ampli) avec entrée PTT.
* **LEDs** et résistances (220Ω) :
    * 1 LED pour le statut GPS (fix).
    * 1 LED pour le statut d'émission (TX).
    * 1 LED pour chaque bande utilisée (ex: 9 LEDs pour 9 bandes).

***

## 🔌 Schéma de Câblage

| Composant          | Broche du Composant | Broche de l'Arduino Mega 2560 |
| :----------------- | :------------------ | :---------------------------- |
| **Module GPS** | TX (Transmit)       | **19 (RX1)** |
| **LED Statut GPS** | Anode (+)           | **22** |
| **LED Émission (TX)** | Anode (+)           | **23** |
| **Commande PTT** | Signal PTT          | **24** |
| **DDS AD9850** | DATA                | **30** |
|                    | W\_CLK              | **26** |
|                    | FQ\_UD              | **28** |
|                    | RESET               | **32** |
| **Filtre 80m** | Commande            | **34** |
| **LED Bande 80m** | Anode (+)           | **43** |
| **Filtre 40m** | Commande            | **35** |
| **LED Bande 40m** | Anode (+)           | **44** |
| **... (etc)** | ...                 | **...** |

*N'oubliez pas de connecter toutes les cathodes des LEDs et les masses (GND) des modules à la masse commune de l'Arduino.*

***

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

***

## 📄 Génération de la Documentation

Ce projet est prêt pour [Doxygen](https://www.doxygen.nl/). Pour générer la documentation HTML du code source :

1.  **Installez Doxygen** et (optionnellement) **Graphviz** pour les diagrammes.
2.  Ouvrez un terminal à la racine du projet.
3.  Exécutez la commande :
    ```sh
    doxygen Doxyfile
    ```
4.  Ouvrez le fichier `index.html` situé dans le dossier `doxygen_html` nouvellement créé.

***

## 📂 Structure du Projet

Le projet est organisé autour des fichiers suivants pour une meilleure clarté :

* `balise_hf.ino` : Le **fichier principal** qui orchestre tout le projet.
* `GestionnaireGPS.h` : La déclaration de la classe qui gère la communication avec le **module GPS**.
* `GestionnaireGPS.cpp` : L'implémentation (le code) de la classe `GestionnaireGPS`.
* `GestionnaireDDS.h` : La déclaration de la classe qui **pilote le générateur de fréquence** AD9850.
* `GestionnaireDDS.cpp` : L'implémentation de la classe `GestionnaireDDS`.
* `EmetteurMorse.h` : La déclaration de la classe qui se charge de **traduire et d'envoyer le code Morse**.
* `EmetteurMorse.cpp` : L'implémentation de la classe `EmetteurMorse`.
* `Balise.h` : La déclaration de la classe qui sert de **"fiche de réglages"** pour chaque bande.
* `Balise.cpp` : L'implémentation de la classe `Balise`.
* `Doxyfile` : Le fichier de **configuration pour la documentation** Doxygen.
* `README.md` : Ce fichier d'information.