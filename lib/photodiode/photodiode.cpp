#include "photodiode.h"
#include <Arduino.h>

Photodiode::Photodiode(uint8_t adcPin) : _adcPin(adcPin) {}

void Photodiode::begin() {
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
}

int Photodiode::readRaw() {
    return analogRead(_adcPin);
}

float Photodiode::readVoltage() {
    int raw = analogRead(_adcPin);
    return (3.3 * raw) / 4095.0;
}
