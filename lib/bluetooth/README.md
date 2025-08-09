# Bluetooth (BLE) module

This library implements a simple BLE UART bridge using NimBLE on ESP32‑C3. It advertises the Nordic UART Service (NUS) so common mobile apps/tools can connect and exchange text.

## GATT layout
- Service: NUS `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`
- Characteristics:
  - TX (Notify): `6E400003-B5A3-F393-E0A9-E50E24DCCA9E`
    - Direction: device → central (notifications)
  - RX (Write/WriteNR): `6E400002-B5A3-F393-E0A9-E50E24DCCA9E`
    - Direction: central → device (writes)

## Public API (BluetoothManager)
- `begin(const char* name)`: initializes NimBLE, creates the service/characteristics, and starts advertising.
- `send(const String& data)`: sends a notification on TX when a client is connected.
- `available() const`: returns true if any data has been received on RX and buffered.
- `receive()`: returns and clears the buffered incoming data.

## Internal structure
- Anonymous namespace (file‑local):
  - `NimBLEServer* server`: BLE server instance.
  - `NimBLECharacteristic* tx`: TX characteristic handle.
  - `String rxBuf`: buffer for concatenated incoming RX data (ASCII by default).
  - `volatile bool connected`: connection flag updated by server callbacks.

- Callbacks:
  - `SrvCb : NimBLEServerCallbacks`
    - `onConnect(NimBLEServer*)`: sets `connected = true`.
    - `onDisconnect(NimBLEServer*)`: sets `connected = false` and restarts advertising.
  - `RxCb : NimBLECharacteristicCallbacks`
    - `onWrite(NimBLECharacteristic*)`: appends received bytes from RX to `rxBuf`.

- Advertising: adds the service UUID and enables scan response; advertising restarts on disconnect.

## Why the anonymous namespace?
It restricts those globals and helper classes to this translation unit (equivalent to giving them internal linkage). This prevents accidental symbol clashes across files. You could make them `static` instead; the anonymous namespace is a clean C++ way to keep the module self‑contained. It’s not strictly required, but recommended.

## Notes
- RX data handling assumes ASCII. If you need binary, store `getValue()` contents without casting to `char*`.
- TX uses notifications; most central apps must enable notifications on the TX characteristic to receive data.
- Requires PlatformIO `lib_deps = h2zero/NimBLE-Arduino`.
