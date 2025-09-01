#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include <Arduino.h>


class MPU6050 {
public:
  MPU6050();
  void begin(uint8_t sda, uint8_t scl);
  bool testConnection();
  void getAcceleration(int16_t* ax, int16_t* ay, int16_t* az);
};

#endif
