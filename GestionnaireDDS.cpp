#include "GestionnaireDDS.h"

GestionnaireDDS::GestionnaireDDS(int w_clk, int fq_ud, int data, int reset) :
  _pin_w_clk(w_clk), _pin_fq_ud(fq_ud), _pin_data(data), _pin_reset(reset) {}

void GestionnaireDDS::begin() {
  pinMode(_pin_w_clk, OUTPUT);
  pinMode(_pin_fq_ud, OUTPUT);
  pinMode(_pin_data, OUTPUT);
  pinMode(_pin_reset, OUTPUT);
  digitalWrite(_pin_w_clk, LOW);
  digitalWrite(_pin_fq_ud, LOW);
  digitalWrite(_pin_reset, HIGH);
  digitalWrite(_pin_reset, LOW);
}

void GestionnaireDDS::setFrequency(unsigned long frequency) {
  uint32_t freq_word = (uint32_t)((double)frequency * (double)(1ULL << 32) / _dds_clock);
  sendCommand((uint64_t)freq_word);
  pulseFqUd();
}

void GestionnaireDDS::pulseFqUd() {
  digitalWrite(_pin_fq_ud, HIGH);
  digitalWrite(_pin_fq_ud, LOW);
}

void GestionnaireDDS::sendCommand(uint64_t w) {
  for (int i = 0; i < 40; i++, w >>= 1) {
    digitalWrite(_pin_data, w & 1);
    digitalWrite(_pin_w_clk, HIGH);
    digitalWrite(_pin_w_clk, LOW);
  }
}