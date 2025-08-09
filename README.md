# ESP32-C3-MINI Sensor + Emitter Project

## Goal
- Read photodiode via ADC and convert to volts.
- Read MPU6050 via I2C (0x68) and report acceleration.
- Drive an IR emitter with PWM duty cycle using LEDC (~38 kHz typical carrier).
- Transmit readings over BLE (ESP32‑C3 does not support Classic BluetoothSerial).

## Board / Tooling
- Board: AirM2M CORE ESP32‑C3 (esp32c3)
- Framework: Arduino (framework-arduinoespressif32)
- Build system: PlatformIO (VS Code recommended)

## Project structure
- `src/`: application entry (`main.cpp`).
- `include/`: public headers.
- `test/`: Unity tests (Arduino-style, each test firmware defines `setup()`/`loop()`).
- `lib/`: project libraries; each library in its own subfolder:
  - `lib/<name>/src/*.cpp`, `lib/<name>/include/*.h` (or provide a `library.json`).
  - Files placed directly under `lib/` (no subfolder) are ignored by PlatformIO.

## Current state (Aug 2025)
- Build config: `platformio.ini` uses env `airm2m_core_esp32c3`, NimBLE-Arduino `^1.4.1`.
- Tests: `test/test.cpp` is a minimal smoke test. `test_build_src = false` prevents `src/main.cpp` from linking into test firmwares.
- Modules:
  - Photodiode: class reads ADC and computes voltage
  
  - MPU6050: class initializes device @ 0x68 and reads acceleration.
  - Emitter: class configures LEDC PWM for duty control (target ~38 kHz configurable).
  - Bluetooth: BLE implemented with NimBLE-UART (Nordic UART Service), with corrected callback signatures for ESP32‑C3.
- Library layout: modules live under `lib/<module>/...` so PlatformIO compiles them; flat files under `lib/` are not used.

## PlatformIO configuration (platformio.ini)
Minimal:
```
platform = espressif32
board = airm2m_core_esp32c3
framework = arduino
lib_deps = h2zero/NimBLE-Arduino @ ^1.4.1
test_build_src = false
```
Optional:
```
monitor_speed = 115200
upload_port = COMx
; test_port = COMx
```

## Setup
1) Open this folder in VS Code.
2) Install the "PlatformIO IDE" extension.
3) Open a PlatformIO terminal: View → Command Palette → "PlatformIO: New Terminal".

## Build
- Build application (no upload):
```
pio run -e airm2m_core_esp32c3
```
- Clean:
```
pio run -e airm2m_core_esp32c3 -t clean
```
- Upload (board connected):
```
pio run -e airm2m_core_esp32c3 -t upload
```
- Serial monitor (adjust):
```
pio device monitor --baud 115200 --port COMx
```

## Run unit tests
- Keep only one test file in `test/` that defines `setup()/loop()` (e.g., `test/test.cpp`).
- Ensure `test_build_src = false` to avoid linking `src/main.cpp` into tests.
- Build-only (no upload/run):
```
pio test -e airm2m_core_esp32c3 --without-uploading --without-testing
```
- Run on device (board connected):
```
pio test -e airm2m_core_esp32c3 --test-port COMx
```

## Known issues / troubleshooting
- Multiple definition of `setup/loop`:
  - Cause: `src/main.cpp` and a test both define `setup/loop` during test build.
  - Fix: `test_build_src = false` and only one active test firmware in `test/`.
- Undefined references to your classes:
  - Cause: `.cpp` files placed directly under `lib/` root are ignored.
  - Fix: move into `lib/<libname>/src` (and headers to `lib/<libname>/include`) or put all sources in `src/`.
- ESP32‑C3 Bluetooth:
  - Classic BluetoothSerial is not supported on C3. Use BLE (NimBLE-Arduino). Match callback signatures to the installed NimBLE version.
- I2C `requestFrom` overload warnings:
  - Prefer explicit types, e.g., `requestFrom((uint8_t)0x68, (size_t)6, true)` to silence ambiguity on ESP32 cores.

## To‑Do
- Finalize header includes (`#include <Arduino.h>`) where `uint8_t/uint32_t` are used.
- Add configuration for I2C pins and emitter PWM channel/frequency for ESP32‑C3.
- Add unit tests (compile-only) for each module; add integration test once hardware is connected.
- Document wiring/pin map for photodiode and MPU6050 (SDA/SCL pins).
