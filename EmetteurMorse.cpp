#include "EmetteurMorse.h"
#include <string.h> // On a besoin de cet outil pour calculer la longueur d'un message.

/**
 * @brief Dictionnaire de traduction de l'alphabet latin vers le Morse.
 * @details Ce grand tableau est notre dictionnaire. L'astuce est que l'on peut
 * trouver directement le code morse d'un caractère en utilisant son code ASCII
 * comme numéro de case (indice) dans le tableau.
 */
static const char* const MORSE_CODE[] = {
    // Les premières cases sont vides car elles correspondent à des caractères non imprimables.
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    // Cases 48 à 57 : Chiffres de '0' à '9'
    /* '0' */ "-----",   /* '1' */ ".----",   /* '2' */ "..---",   /* '3' */ "...--",   /* '4' */ "....-",
    /* '5' */ ".....",   /* '6' */ "-....",   /* '7' */ "--...",   /* '8' */ "---..",   /* '9' */ "----.",
    "", "", "", "", "", "", "", // Autres cases vides pour les symboles entre les chiffres et les lettres.
    // Cases 65 à 90 : Lettres de 'A' à 'Z'
    /* 'A' */ ".-",      /* 'B' */ "-...",    /* 'C' */ "-.-.",    /* 'D' */ "-..",     /* 'E' */ ".",
    /* 'F' */ "..-.",    /* 'G' */ "--.",     /* 'H' */ "....",    /* 'I' */ "..",      /* 'J' */ ".---",
    /* 'K' */ "-.-",     /* 'L' */ ".-..",    /* 'M' */ "--",      /* 'N' */ "-.",      /* 'O' */ "---",
    /* 'P' */ ".--.",    /* 'Q' */ "--.-",    /* 'R' */ ".-.",     /* 'S' */ "...",     /* 'T' */ "-",
    /* 'U' */ "..-",     /* 'V' */ "...-",    /* 'W' */ ".--",     /* 'X' */ "-..-",    /* 'Y' */ "-.--",
    /* 'Z' */ "--.."
};

EmetteurMorse::EmetteurMorse(int pttPin, int txLedPin, int wpm) 
  : _pttPin(pttPin), _txLedPin(txLedPin) {
  // On prépare les broches à fonctionner en sortie.
  pinMode(_pttPin, OUTPUT);
  pinMode(_txLedPin, OUTPUT);
  // On s'assure que l'émission est coupée au départ.
  pttOff();
  // On calcule la durée d'un "point" en millisecondes. C'est la base de tout le rythme Morse.
  // La formule standard est 1200 divisé par la vitesse en mots par minute.
  _dot_len_ms = 1200 / wpm;
}

// Active l'émission.
void EmetteurMorse::pttOn() {
  digitalWrite(_pttPin, HIGH);
  digitalWrite(_txLedPin, HIGH);
}

// Coupe l'émission.
void EmetteurMorse::pttOff() {
  digitalWrite(_pttPin, LOW);
  digitalWrite(_txLedPin, LOW);
}

// Envoie un "point".
void EmetteurMorse::dot() {
  pttOn();
  delay(_dot_len_ms); // Le "point" dure 1 unité de temps.
  pttOff();
  delay(_dot_len_ms); // L'espace entre deux sons dans une même lettre dure 1 unité de temps.
}

// Envoie un "trait".
void EmetteurMorse::dash() {
  pttOn();
  delay(_dot_len_ms * 3); // Le "trait" dure 3 unités de temps.
  pttOff();
  delay(_dot_len_ms); // L'espace entre deux sons dure toujours 1 unité de temps.
}

void EmetteurMorse::sendMessage(const char* message) {
  pttOff();
  size_t longueurMessage = strlen(message);

  // On parcourt chaque caractère du message, un par un.
  for (size_t i = 0; i < longueurMessage; i++) {
    char c = toupper(message[i]); // On met en majuscule pour correspondre à notre dictionnaire.

    if (c == ' ') {
      // Si c'est un espace, on fait une pause plus longue (un espace entre mots).
      delay(_dot_len_ms * 4);
    } else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
      // Si c'est une lettre ou un chiffre, on l'envoie.
      sendChar(c);
      // Puis on attend l'équivalent d'un espace entre deux lettres.
      delay(_dot_len_ms * 2);
    }
  }
  delay(1000); // Petite pause à la fin de la transmission complète.
}

void EmetteurMorse::sendChar(char c) {
  // On va chercher la traduction dans notre dictionnaire.
  const char* code = MORSE_CODE[(int)c];
  // On "joue" la séquence de points et de traits trouvée.
  while (*code) {
    if (*code == '.') dot();
    else if (*code == '-') dash();
    code++;
  }
}