#include "photodiode.h"
#include "emitter.h"
#include "mpu6050.h"
#include "bluetooth.h"

Photodiode photodiode(2);    // GPIO2 for ADC
Emitter irLED(3);            // GPIO3 for PWM
BluetoothManager bt;
MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  irLED.begin();
  irLED.setDutyPercent(50);
  photodiode.begin();

  mpu.begin(6, 7); // SDA = GPIO6, SCL = GPIO7

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
  }

  bt.begin("AntiSleepGlasses");
  Serial.println("Bluetooth Started. Waiting for connections...");
}

void loop() {
  int raw = photodiode.readRaw();
  float voltage = photodiode.readVoltage();
  int16_t ax, ay, az;

  mpu.getAcceleration(&ax, &ay, &az);

  Serial.printf("Raw: %d\tVoltage: %.2f V\n", raw, voltage);
  Serial.printf("AX:%d,AY:%d,AZ:%d\n", ax, ay, az);

  String sensorData = "Raw:" + String(raw) + "\tVoltage:" + String(voltage) + "V\tAX:" + String(ax) + ",AY:" + String(ay) + ",AZ:" + String(az);
  bt.send(sensorData);

  if (bt.available()) {
    String received = bt.receive();
    Serial.print("Received via BT: ");
    Serial.println(received);
  }

  delay(200);
}