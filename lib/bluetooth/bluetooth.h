#pragma once
#include <Arduino.h>

// BLE UART-like wrapper (NimBLE on ESP32-C3)
class BluetoothManager {
public:
  BluetoothManager();
  void begin(const char* deviceName);
  void send(const String& data);
  bool available() const;
  String receive();
};
