#ifndef BALISE_H
#define BALISE_H

#include <Arduino.h>

/**
 * @class Balise
 * @brief Une "fiche de réglages" pour une émission.
 * @details Cette classe n'est pas un "travailleur" mais plutôt un conteneur,
 * une sorte de fiche cartonnée qui regroupe toutes les informations
 * nécessaires pour une émission sur une bande spécifique : le message, la
 * fréquence, et les broches à activer.
 */
class Balise {
public:
  /**
   * @brief Remplit la "fiche de réglages".
   * @param message Le texte à envoyer.
   * @param frequencyMhz La fréquence à utiliser, en Mégahertz.
   * @param filterPin La broche du filtre pour cette fréquence.
   */
  Balise(const char* message, double frequencyMhz, int filterPin);

  /**
   * @brief Permet de lire le message écrit sur la fiche.
   */
  const char* getMessage() const;

  /**
   * @brief Permet de lire la fréquence écrite sur la fiche (en Hertz).
   */
  unsigned long getFrequency() const;

  /**
   * @brief Permet de lire le numéro de la broche du filtre.
   */
  int getFilterPin() const;
  

private:
  const char* _message;       ///< Le message à émettre.
  unsigned long _frequencyHz; ///< La fréquence, stockée en Hertz.
  int _filterPin;             ///< La broche du filtre.
};

#endif