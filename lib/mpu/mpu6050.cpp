#include <Wire.h>
#include <Arduino.h>
#include "mpu6050.h"

MPU6050::MPU6050() {}

void MPU6050::begin(uint8_t sda, uint8_t scl) {
    Wire.begin(sda, scl);
}

bool MPU6050::testConnection() {
    Wire.beginTransmission(0x68);
    return Wire.endTransmission() == 0;
}

void MPU6050::getAcceleration(int16_t* ax, int16_t* ay, int16_t* az) {
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);   
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true); //TODO: verify pin for request
    *ax = (Wire.read() << 8) | Wire.read();
    *ay = (Wire.read() << 8) | Wire.read();
    *az = (Wire.read() << 8) | Wire.read();
}
