#ifndef EMITTER_H
#define EMITTER_H

#include <Arduino.h> 

class Emitter {
public:
  Emitter(uint8_t pwmPin, uint8_t channel = 0, uint32_t freq = 38000, uint8_t resolution = 8);
  void begin();
  void setDutyPercent(uint8_t percent);

private:
  uint8_t _pwmPin, _channel, _resolution;
  uint32_t _freq;
};

#endif
