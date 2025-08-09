#include "emitter.h"
#include <Arduino.h>

Emitter::Emitter(uint8_t pwmPin, uint8_t channel, uint32_t freq, uint8_t resolution)
    : _pwmPin(pwmPin), _channel(channel), _freq(freq), _resolution(resolution) {}

void Emitter::begin() {
    ledcSetup(_channel, _freq, _resolution);
    ledcAttachPin(_pwmPin, _channel);
    setDutyPercent(50);
}

void Emitter::setDutyPercent(uint8_t percent) {
    percent = constrain(percent, 0, 100);
    uint32_t maxDuty = (1 << _resolution) - 1;
    uint32_t duty = (percent * maxDuty) / 100;
    ledcWrite(_channel, duty);
}