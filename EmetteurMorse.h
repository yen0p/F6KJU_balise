#ifndef EMETTEUR_MORSE_H
#define EMETTEUR_MORSE_H

#include <Arduino.h>

/**
 * @class EmetteurMorse
 * @brief Le "télégraphiste" expert en code Morse.
 * @details Cette classe sait comment convertir du texte en signaux Morse. Elle connaît
 * l'alphabet, gère le rythme des points et des traits, et commande physiquement
 * l'émetteur (via le PTT) et la LED d'émission.
 */
class EmetteurMorse {
public:
  /**
   * @brief Construit notre "télégraphiste".
   * @param pttPin La broche pour "appuyer sur le bouton du micro" (PTT).
   * @param txLedPin La broche pour allumer la LED rouge d'émission.
   * @param wpm Le rythme de travail, en mots par minute (MPM).
   */
  EmetteurMorse(int pttPin, int txLedPin, int wpm);

  /**
   * @brief Prend un message et le transmet en Morse du début à la fin.
   * @param message Le texte à envoyer.
   */
  void sendMessage(const char* message);

private:
  /**
   * @brief Interne : traduit et envoie un seul caractère.
   * @param c La lettre ou le chiffre à envoyer.
   */
  void sendChar(char c);

  /**
   * @brief Génère le son court ("point") du Morse.
   */
  void dot();

  /**
   * @brief Génère le son long ("trait") du Morse.
   */
  void dash();

  /**
   * @brief Appuie sur le bouton du PTT et allume la LED d'émission.
   */
  void pttOn();

  /**
   * @brief Relâche le bouton du PTT et éteint la LED d'émission.
   */
  void pttOff();
  
  int _pttPin;        ///< Mémorise la broche du PTT.
  int _txLedPin;      ///< Mémorise la broche de la LED d'émission.
  int _dot_len_ms;    ///< Calcule et mémorise la durée d'un "point" en millisecondes, qui définit tout le rythme.
};

#endif