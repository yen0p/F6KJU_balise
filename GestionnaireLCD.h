#ifndef GESTIONNAIRE_LCD_H
#define GESTIONNAIRE_LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/**
 * @enum StatutBalise
 * @brief Définit les différents états possibles de la balise pour l'affichage.
 */
enum StatutBalise {
  STATUT_INITIAILISATION,
  STATUT_ATTENTE_GPS,
  STATUT_PRET,
  STATUT_EMISSION,
  STATUT_ERREUR,
};

/**
 * @class GestionnaireLCD
 * @brief Pilote l'écran LCD I2C pour afficher les informations de la balise.
 * @details Cette classe est une interface complète pour l'écran. Elle formate et
 * affiche toutes les données pertinentes sur les 4 lignes de l'afficheur,
 * en s'assurant que l'écran est toujours à jour et lisible.
 */
class GestionnaireLCD {
public:
  GestionnaireLCD(uint8_t address, uint8_t cols, uint8_t rows);
  void begin();

  /**
   * @brief Met à jour l'affichage de la date et de l'heure sur la première ligne.
   * @param jour Le jour du mois actuel (1-31).
   * @param mois Le mois actuel (1-12).
   * @param annee L'année actuelle (ex: 2025).
   * @param h L'heure actuelle (0-23).
   * @param m Les minutes actuelles (0-59).
   * @param s Les secondes actuelles (0-59).
   */
  void afficherDateHeure(uint8_t jour, uint8_t mois, uint16_t annee, uint8_t h, uint8_t m, uint8_t s);
  void mettreAJourPositionEtLocator(double lat, double lon, const char* locator);
  void mettreAJourStatus(StatutBalise status);
  void displayBeaconInfo(const char* callsign, double frequencyMhz);
  void clearBeaconInfo();

private:
  LiquidCrystal_I2C _lcd;
  uint8_t _cols;
};

#endif