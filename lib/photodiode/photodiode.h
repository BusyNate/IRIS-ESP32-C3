#ifndef PHOTODIODE_ADC_H
#define PHOTODIODE_ADC_H

#include <Arduino.h>

class Photodiode {
public:
  Photodiode(uint8_t adcPin);
  void begin();
  int readRaw();
  float readVoltage();
private:
  uint8_t _adcPin;
};

#endif
