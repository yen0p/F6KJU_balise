#ifndef GESTIONNAIRE_DDS_H
#define GESTIONNAIRE_DDS_H

#include <Arduino.h>

/**
 * @class GestionnaireDDS
 * @brief Le pilote du composant qui génère le signal radio (DDS AD9850).
 * @details Cette classe est une "télécommande" pour le module AD9850. Elle simplifie
 * son utilisation en cachant les détails techniques complexes. On peut simplement
 * lui demander de générer une fréquence, et elle se charge d'envoyer les bons
 * ordres au composant.
 */
class GestionnaireDDS {
public:
  /**
   * @brief Construit la "télécommande" pour le DDS.
   * @param w_clk Broche pour l'horloge de chargement des mots.
   * @param fq_ud Broche pour la mise à jour de la fréquence.
   * @param data Broche pour l'envoi des données.
   * @param reset Broche pour réinitialiser le composant.
   */
  GestionnaireDDS(int w_clk, int fq_ud, int data, int reset);

  /**
   * @brief Allume et prépare le module DDS.
   * @details Doit être appelée une fois au démarrage pour s'assurer que le DDS
   * est prêt à recevoir des ordres.
   */
  void begin();

  /**
   * @brief Donne l'ordre au DDS de produire une fréquence précise.
   * @param frequency La fréquence à générer, en Hertz (ex: 14095600).
   */
  void setFrequency(unsigned long frequency);

private:
  /**
   * @brief Envoie un "mot de code" de 40 bits au DDS.
   * @details C'est la communication de bas niveau. La fréquence est convertie en un
   * long code binaire que le DDS peut comprendre.
   * @param w Le code de 40 bits à envoyer.
   */
  void sendCommand(uint64_t w);

  /**
   * @brief Valide la nouvelle fréquence.
   * @details Après avoir envoyé le code, une petite impulsion sur cette broche dit au
   * DDS : "Ok, c'est bon, tu peux utiliser cette nouvelle fréquence maintenant".
   */
  void pulseFqUd();

  int _pin_w_clk, _pin_fq_ud, _pin_data, _pin_reset; ///< Mémorise les broches de connexion.
  const double _dds_clock = 125000000.0; ///< La vitesse de l'horloge interne du module DDS, nécessaire pour les calculs.
};

#endif