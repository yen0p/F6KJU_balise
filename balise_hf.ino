/**
 * @file balise_hf.ino
 * @brief Programme principal qui orchestre le fonctionnement de la balise Morse.
 * @details Ce fichier est le "cerveau" du projet. Il initialise tous les composants
 * au démarrage, puis entre dans une boucle infinie où il attend le bon moment
 * (le début de chaque minute) pour déclencher une émission Morse sur une
 * bande radio-amateur.
 */

// On inclut les "plans de construction" de nos objets personnalisés.
// Chaque fichier .h est comme une notice de montage pour un composant logiciel.
#include "GestionnaireGPS.h"
#include "GestionnaireDDS.h"
#include "EmetteurMorse.h"
#include "Balise.h"

// --- CONFIGURATION MATERIELLE : C'est ici qu'on dit au programme où sont branchés les composants ---

// -- Broches pour les indicateurs visuels et la commande d'émission --
const int BROCHE_LED_GPS_LOCK = 22; ///< Une LED qui s'allume quand le GPS a trouvé sa position.
const int BROCHE_LED_EMISSION = 23; ///< Une LED qui s'allume quand la balise est en train d'émettre.
const int BROCHE_PTT = 24;          ///< La broche qui commande l'ampli pour passer en émission (Push-To-Talk).

// -- Broches pour le composant qui génère la fréquence radio (DDS) --
const int BROCHE_DDS_W_CLK = 26;
const int BROCHE_DDS_FQ_UD = 28;
const int BROCHE_DDS_DATA = 30;
const int BROCHE_DDS_RESET = 32;

// -- Broches pour les filtres et les LEDs de chaque bande --
// Pour chaque bande, on a une broche pour activer le bon filtre et une autre pour allumer une LED.
const int BROCHE_FILTRE_80M = 34; const int BROCHE_LED_80M = 43;
const int BROCHE_FILTRE_40M = 35; const int BROCHE_LED_40M = 44;
const int BROCHE_FILTRE_30M = 36; const int BROCHE_LED_30M = 45;
const int BROCHE_FILTRE_20M = 37; const int BROCHE_LED_20M = 46;
const int BROCHE_FILTRE_17M = 38; const int BROCHE_LED_17M = 47;
const int BROCHE_FILTRE_15M = 39; const int BROCHE_LED_15M = 48;
const int BROCHE_FILTRE_12M = 40; const int BROCHE_LED_12M = 49;
const int BROCHE_FILTRE_10M = 41; const int BROCHE_LED_10M = 50;
const int BROCHE_FILTRE_6M  = 42; const int BROCHE_LED_6M  = 51;

// --- CONFIGURATION DU RYTHME DU MORSE ---
const int MORSE_MOT_PAR_MINUTE = 15; ///< Vitesse de la transmission Morse.

// --- CRÉATION DES OBJETS : On "fabrique" nos assistants logiciels ---
// Chaque objet est un spécialiste dans son domaine.
GestionnaireGPS gestionGPS(Serial1); // L'expert qui sait parler au module GPS.
GestionnaireDDS gestionDDS(BROCHE_DDS_W_CLK, BROCHE_DDS_FQ_UD, BROCHE_DDS_DATA, BROCHE_DDS_RESET); // Le pilote du générateur de fréquence.
EmetteurMorse   emetteurMorse(BROCHE_PTT, BROCHE_LED_EMISSION, MORSE_MOT_PAR_MINUTE); // Le télégraphiste qui connaît le Morse.

// --- LISTE DES EMISSIONS À FAIRE : Le programme de la balise ---
// On définit ici chaque émission que la balise devra faire, l'une après l'autre.
// Format : { "Message à envoyer", Fréquence en MHz, Broche du Filtre, Broche de la LED }
Balise balises[] = {
  Balise("CQ DE F6KJU #LOCA#", 3.579,   BROCHE_FILTRE_80M, BROCHE_LED_80M),
  Balise("CQ DE F6KJU #LOCA#", 7.0475,  BROCHE_FILTRE_40M, BROCHE_LED_40M),
  Balise("CQ DE F6KJU #LOCA#", 10.140,  BROCHE_FILTRE_30M, BROCHE_LED_30M),
  Balise("CQ DE F6KJU #LOCA#", 14.0956, BROCHE_FILTRE_20M, BROCHE_LED_20M),
  Balise("CQ DE F6KJU #LOCA#", 18.1046, BROCHE_FILTRE_17M, BROCHE_LED_17M),
  Balise("CQ DE F6KJU #LOCA#", 21.0946, BROCHE_FILTRE_15M, BROCHE_LED_15M),
  Balise("CQ DE F6KJU #LOCA#", 24.9246, BROCHE_FILTRE_12M, BROCHE_LED_12M),
  Balise("CQ DE F6KJU #LOCA#", 28.1246, BROCHE_FILTRE_10M, BROCHE_LED_10M),
  Balise("CQ DE F6KJU #LOCA#", 50.065,  BROCHE_FILTRE_6M,  BROCHE_LED_6M)
};
// On compte automatiquement combien il y a de balises dans notre liste.
const int NOMBRE_DE_BALISES = sizeof(balises) / sizeof(balises[0]);
// On crée des "marque-pages" pour savoir où on en est.
int indexBaliseCourante = 0;      // Pour se souvenir de la prochaine balise à émettre.
bool aEteTransmisCetteMinute = false; // Pour s'assurer de n'émettre qu'une fois par minute.

/**
 * @brief Fonction d'initialisation, exécutée une seule fois au démarrage de l'Arduino.
 */
void setup() {
  // Ouvre la communication avec l'ordinateur pour pouvoir afficher des messages de statut.
  Serial.begin(115200);
  Serial.println(F("Initialisation de la balise Morse F6KJU..."));

  // On prépare toutes les broches des LEDs et des filtres à être des sorties de courant.
  pinMode(BROCHE_LED_GPS_LOCK, OUTPUT);
  pinMode(BROCHE_LED_EMISSION, OUTPUT);
  // On s'assure qu'elles sont toutes éteintes au démarrage.
  digitalWrite(BROCHE_LED_GPS_LOCK, LOW);
  digitalWrite(BROCHE_LED_EMISSION, LOW);
  for (int i = 0; i < NOMBRE_DE_BALISES; ++i) {
    pinMode(balises[i].getFilterPin(), OUTPUT);
    pinMode(balises[i].getLedPin(), OUTPUT);
    digitalWrite(balises[i].getFilterPin(), LOW);
    digitalWrite(balises[i].getLedPin(), LOW);
  }

  // On demande à notre pilote de DDS de préparer le composant.
  gestionDDS.begin();
  
  // -- Attente de la synchronisation GPS --
  Serial.println(F("En attente de la synchronisation GPS... (Ceci peut prendre quelques minutes)"));
  // On boucle ici tant que le GPS ne nous a pas donné une heure et une position fiables.
  while (!gestionGPS.hasValidTime() || !gestionGPS.hasValidLocation()) {
    gestionGPS.update(); // On demande à notre expert GPS de continuer à écouter.
  }
  
  // Le GPS est prêt ! On allume la LED verte pour le signaler.
  digitalWrite(BROCHE_LED_GPS_LOCK, HIGH);
  Serial.println(F("GPS synchronisé ! La balise est prête."));
}

/**
 * @brief Boucle principale, exécutée en continu après le setup().
 */
void loop() {
  // À chaque passage, on demande à notre expert GPS d'écouter si de nouvelles infos sont arrivées.
  gestionGPS.update();

  // La condition principale : est-ce le bon moment pour émettre ?
  // On vérifie si 1) l'heure est valide, 2) on est à la seconde "0" d'une minute, 3) on n'a pas déjà émis.
  if (gestionGPS.hasValidTime() && gestionGPS.getSeconds() == 0 && !aEteTransmisCetteMinute) {
    // C'est le bon moment ! On lève notre drapeau pour ne pas ré-émettre dans la même seconde.
    aEteTransmisCetteMinute = true;

    // On récupère la "fiche de réglages" de la balise à émettre cette minute.
    Balise& baliseCourante = balises[indexBaliseCourante];
    
    // -- Préparation visuelle --
    // On éteint toutes les LEDs de bande pour être sûr qu'il n'y a pas de confusion.
    for (int i = 0; i < NOMBRE_DE_BALISES; ++i) {
        digitalWrite(balises[i].getLedPin(), LOW);
    }
    // On allume la LED correspondant à la bande qu'on va utiliser.
    digitalWrite(baliseCourante.getLedPin(), HIGH);
    
    // -- Préparation du message --
    // On crée un objet "String" à partir du message de base.
    String messageFinal = String(baliseCourante.getMessage());
    // On utilise la méthode très pratique ".replace()" pour insérer le locator actuel.
    messageFinal.replace("#LOCA#", gestionGPS.getLocator());
    
    // -- Préparation de l'émission radio --
    // On active le bon filtre pour la bande choisie.
    digitalWrite(baliseCourante.getFilterPin(), HIGH);
    // On ordonne au pilote du DDS de générer la bonne fréquence.
    gestionDDS.setFrequency(baliseCourante.getFrequency());
    delay(100); // Petite pause pour laisser le temps aux composants de se stabiliser.

    // -- ÉMISSION --
    Serial.print(F("Emission du message : "));
    Serial.println(messageFinal);
    // On demande à notre télégraphiste d'envoyer le message final.
    emetteurMorse.sendMessage(messageFinal.c_str());

    // -- Nettoyage et préparation pour la prochaine minute --
    // On désactive le filtre.
    digitalWrite(baliseCourante.getFilterPin(), LOW);
    // On passe à la balise suivante de notre liste (et on revient au début si on est à la fin).
    indexBaliseCourante = (indexBaliseCourante + 1) % NOMBRE_DE_BALISES;
  }

  // Dès qu'on passe à la seconde "1", on réinitialise notre drapeau.
  // La balise est de nouveau prête à émettre quand la prochaine minute commencera.
  if (gestionGPS.getSeconds() > 0) {
    aEteTransmisCetteMinute = false;
  }
}