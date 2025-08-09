#include "Balise.h"

// Le constructeur prend les informations et les range soigneusement dans les "cases" de l'objet.
Balise::Balise(const char* message, double frequencyMhz, int filterPin)
  : _message(message), _filterPin(filterPin) {
    // On convertit la fréquence de MHz en Hertz, car c'est le format que notre
    // pilote de DDS attend.
    _frequencyHz = (unsigned long)(frequencyMhz * 1000000.0);
}

// Les fonctions suivantes permettent simplement de récupérer les informations stockées.
const char* Balise::getMessage() const { return _message; }
unsigned long Balise::getFrequency() const { return _frequencyHz; }
int Balise::getFilterPin() const { return _filterPin; }