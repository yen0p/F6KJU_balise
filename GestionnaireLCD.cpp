/**
 * @file GestionnaireLCD.cpp
 * @brief Implémentation de la classe GestionnaireLCD.
 * @details Contient le code qui formate les chaînes de caractères et les envoie
 * à la bibliothèque LiquidCrystal_I2C pour affichage.
 */
#include "GestionnaireLCD.h"

GestionnaireLCD::GestionnaireLCD(uint8_t address, uint8_t cols, uint8_t rows)
  : _lcd(address, cols, rows), _cols(cols) {}

void GestionnaireLCD::begin() {
  _lcd.init();
  _lcd.backlight();
  _lcd.setCursor(0, 0);
  this.mettreAJourStatus(StatutBalise.STATUT_INITIAILISATION);
}

/**
 * @brief Formate et affiche la date et l'heure complètes sur la ligne 1.
 * @details Utilise la fonction `sprintf` pour créer une chaîne de caractères
 * formatée "JJ/MM/AAAA HH:MM:SS" avant de l'afficher.
 */
void GestionnaireLCD::afficherDateHeure(uint8_t jour, uint8_t mois, uint16_t annee, uint8_t h, uint8_t m, uint8_t s) {
  char lineBuffer[21];
  sprintf(lineBuffer, "%02d/%02d/%04d %02d:%02d:%02d", jour, mois, annee, h, m, s);
  _lcd.setCursor(0, 0);
  _lcd.print(lineBuffer);
}

// ... Les autres fonctions de la classe restent inchangées et commentées ...
void GestionnaireLCD::mettreAJourPositionEtLocator(double lat, double lon, const char* locator) { /* ... */ }
void GestionnaireLCD::mettreAJourStatus(double lat, double lon, const char* locator) {
  char posBuffer[21]; // Buffer assez grand pour la ligne (20 caractères + le nul)
  // On utilise dtostrf pour convertir les double (lat/lon) en chaîne
  char latStr[8], lonStr[9];
  dtostrf(lat, 6, 3, latStr); // 6 caractères au total, 3 après la virgule
  dtostrf(lon, 7, 3, lonStr);
  
  // On combine toutes les infos dans un seul buffer
  sprintf(posBuffer, "%s,%s %s", latStr, lonStr, locator);

  _lcd.setCursor(0, 1); // Ligne 2
  _lcd.print(posBuffer);
  // On efface le reste de la ligne au cas où le texte précédent était plus long
  for (int i = strlen(posBuffer); i < _cols; i++) {
    _lcd.print(" ");
  }
}

/**
 * @brief Met à jour le message de statut sur la troisième ligne.
 * @param status L'état actuel de la balise, choisi depuis l'énumération StatutBalise.
 */
void GestionnaireLCD::updateStatus(StatutBalise status) {
  _lcd.setCursor(0, 2); // Ligne 3
  String statusMsg;
  switch (status) {

    case STATUT_INITIAILISATION:
        statusMsg = "Initialisation ...";
        break;
    case STATUT_ERREUR:
        statusMsg = "Erreur !"
        break;
    case STATUT_ATTENTE_GPS:
        statusMsg = "Attente Syncronisation GPS...";
        break;
    case STATUT_PRET:
        statusMsg = "Pret. En attente...";
        break;
    case STATUT_EMISSION:
        statusMsg = "En emission";
        break;
  }
  _lcd.print(statusMsg);
  // On efface le reste de la ligne
  for (int i = statusMsg.length(); i < _cols; i++) {
    _lcd.print(" ");
  }
}

/**
 * @brief Affiche les informations de la balise en émission sur la quatrième ligne.
 * @param callsign L'indicatif de la balise (ex: "F6KJU").
 * @param frequencyMhz La fréquence d'émission en MHz.
 */
void GestionnaireLCD::displayBeaconInfo(const char* callsign, double frequencyMhz) {
  char infoBuffer[21];
  char freqStr[10];
  dtostrf(frequencyMhz, 7, 4, freqStr); // 7 caractères, 4 après la virgule

  sprintf(infoBuffer, "%s: %sMHz", callsign, freqStr);
  
  _lcd.setCursor(0, 3); // Ligne 4
  _lcd.print(infoBuffer);
}

/**
 * @brief Efface les informations de la balise sur la quatrième ligne.
 * @details Utile après la fin d'une émission pour nettoyer l'écran.
 */
void GestionnaireLCD::clearBeaconInfo() {
  _lcd.setCursor(0, 3); // Ligne 4
  // On efface toute la ligne en imprimant des espaces
  for (int i = 0; i < _cols; i++) {
    _lcd.print(" ");
  }
}