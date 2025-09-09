#include <NimBLEDevice.h>
#include <Arduino.h>
#include "bluetooth.h"

static const char* UART_SVC = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"; // Nordic UART Service UUID
static const char* UUID_RX  = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"; // RX (Write)
static const char* UUID_TX  = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"; // TX (Notify)

BluetoothManager::BluetoothManager() {}

namespace {
  NimBLEServer* server = nullptr;
  NimBLECharacteristic* tx = nullptr;
  String rxBuf;
  volatile bool connected = false;

  struct SrvCb : NimBLEServerCallbacks {
    void onConnect(NimBLEServer* /pServer/) override {
      connected = true;
    }
    void onDisconnect(NimBLEServer* /pServer/) override {
      connected = false;
      NimBLEDevice::startAdvertising();
    }
  };

  struct RxCb : NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* c) override {
      auto v = c->getValue();
      if (v.length() > 0) {
        // Append as ASCII text; adjust if sending binary
        rxBuf += String((const char*)v.data());
      }
    }
  };
}


void BluetoothManager::begin(const char* name) { 
  NimBLEDevice::init(name);
  server = NimBLEDevice::createServer();
  server->setCallbacks(new SrvCb());

  NimBLEService* svc = server->createService(UART_SVC);
  tx = svc->createCharacteristic(UUID_TX, NIMBLE_PROPERTY::NOTIFY);
  NimBLECharacteristic* rx = svc->createCharacteristic(UUID_RX, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
  rx->setCallbacks(new RxCb());
  svc->start();

  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID(UART_SVC);
  adv->setScanResponse(true);
  NimBLEDevice::startAdvertising();
}

void BluetoothManager::send(const String& data) {
  if (connected && tx) {
    tx->setValue((const uint8_t*)data.c_str(), data.length());
    tx->notify();
  }
}

bool BluetoothManager::available() const {
  return rxBuf.length() > 0;
}

String BluetoothManager::receive() {
  String out = rxBuf;
  rxBuf = "";
  return out;
}
