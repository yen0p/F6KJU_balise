#ifndef GESTIONNAIRE_GPS_H
#define GESTIONNAIRE_GPS_H

#include <Arduino.h>
#include <TinyGPS++.h>

/**
 * @class GestionnaireGPS
 * @brief Spécialiste de la communication avec le module GPS.
 * @details Cette classe est un "traducteur". Elle reçoit les phrases complexes
 * (trames NMEA) envoyées par le module GPS et les déchiffre pour en extraire
 * des informations simples et utilisables : l'heure, la date, la position, etc.
 */
class GestionnaireGPS {
public:
  /**
   * @brief Construit notre "traducteur" GPS.
   * @param serialPort Le canal de communication série (`Serial1`, `Serial2`...)
   * utilisé pour dialoguer avec le module GPS.
   */
  GestionnaireGPS(HardwareSerial& serialPort);

  /**
   * @brief Écoute et déchiffre les données venant du GPS.
   * @details C'est la tâche principale de cet objet. Appelée en boucle, elle lit les
   * données qui arrivent sur le port série et met à jour les informations internes
   * (heure, position...).
   */
  void update();

  /**
   * @brief Permet de savoir si l'heure donnée par le GPS est fiable.
   * @return `true` si l'heure est synchronisée avec les satellites, `false` sinon.
   */
  bool hasValidTime() const;

  /**
   * @brief Permet de savoir si la position donnée par le GPS est fiable.
   * @return `true` si le GPS a une position et qu'elle est récente, `false` sinon.
   */
  bool hasValidLocation() const;

  /**
   * @brief Donne le nombre de secondes de l'heure actuelle.
   * @return Un nombre entre 0 et 59.
   */
  uint8_t getSeconds() const;

  /**
   * @brief Donne la latitude (position Nord/Sud).
   * @return La latitude en degrés.
   */
  double getLatitude() const;

  /**
   * @brief Donne la longitude (position Est/Ouest).
   * @return La longitude en degrés.
   */
  double getLongitude() const;

  /**
   * @brief Donne la position convertie au format "Maidenhead Locator".
   * @details C'est un format de coordonnées très utilisé par les radio-amateurs.
   * @return Un code de 6 caractères (ex: "JN18NT").
   */
  const char* getLocator() const;

private:
  /**
   * @brief Méthode interne pour faire le calcul de conversion des coordonnées.
   * @details Applique une formule mathématique pour transformer la latitude et la
   * longitude en un code de 6 caractères.
   */
  void calculateLocator();

  HardwareSerial& _gpsSerial; ///< Le canal de communication avec le GPS.
  TinyGPSPlus _gps;           ///< L'outil (de la bibliothèque TinyGPS++) qui fait le gros du travail de déchiffrage.
  char _locator[7];           ///< Une petite boîte pour ranger le locator une fois calculé.
};

#endif