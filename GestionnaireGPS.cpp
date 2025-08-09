/**
 * @file GestionnaireGPS.cpp
 * @brief Implémentation de la classe GestionnaireGPS.
 * @details Ce fichier contient le code des fonctions déclarées dans GestionnaireGPS.h.
 * Il gère la logique concrète de lecture des données GPS et de conversion des coordonnées.
 */
#include "GestionnaireGPS.h"
#include <math.h>

/**
 * @brief Construit l'objet et initialise la communication série avec le module GPS.
 */
GestionnaireGPS::GestionnaireGPS(HardwareSerial& serialPort) : _gpsSerial(serialPort) {
  _gpsSerial.begin(9600);
  strcpy(_locator, "......"); // On met une valeur par défaut en attendant les vraies données.
}

/**
 * @brief Boucle de lecture des données NMEA envoyées par le GPS.
 */
void GestionnaireGPS::update() {
  // Tant qu'il y a des données à lire...
  while (_gpsSerial.available() > 0) {
    // On les envoie à la bibliothèque TinyGPS++ qui les déchiffre.
    if (_gps.encode(_gpsSerial.read())) {
      // Si une nouvelle position a été reçue et qu'elle est valide...
      if (_gps.location.isUpdated() && hasValidLocation()) {
        // ... on recalcule le locator.
        calculateLocator();
      }
    }
  }
}

// Implémentation des différentes fonctions "getter".
// Elles se contentent de retourner la valeur correspondante, lue par la bibliothèque TinyGPS++.
bool GestionnaireGPS::hasValidTime() const { return _gps.time.isValid(); }
bool GestionnaireGPS::hasValidLocation() const { return _gps.location.isValid() && _gps.location.age() < 2000; }
uint8_t GestionnaireGPS::getHeure() const { return _gps.time.hour(); }
uint8_t GestionnaireGPS::getMinute() const { return _gps.time.minute(); }
uint8_t GestionnaireGPS::getSeconde() const { return _gps.time.second(); }
uint8_t GestionnaireGPS::getJour() const { return _gps.date.day(); }
uint8_t GestionnaireGPS::getMois() const { return _gps.date.month(); }
uint16_t GestionnaireGPS::getAnnee() const { return _gps.date.year(); }
double GestionnaireGPS::getLatitude() const { return _gps.location.lat(); }
double GestionnaireGPS::getLongitude() const { return _gps.location.lng(); }
const char* GestionnaireGPS::getLocator() const { return _locator; }

/**
 * @brief Fonction interne qui applique la formule de conversion en Maidenhead locator.
 */
void GestionnaireGPS::calculateLocator() {
  double lat = getLatitude(), lon = getLongitude();
  lon += 180; lat += 90;
  _locator[0] = (char)('A' + floor(lon / 20));
  _locator[1] = (char)('A' + floor(lat / 10));
  _locator[2] = (char)('0' + floor(fmod(lon, 20) / 2));
  _locator[3] = (char)('0' + floor(fmod(lat, 10)));
  _locator[4] = (char)('a' + floor(fmod(lon / 2, 1) * 24));
  _locator[5] = (char)('a' + floor(fmod(lat, 1) * 24));
  _locator[6] = '\0';
}