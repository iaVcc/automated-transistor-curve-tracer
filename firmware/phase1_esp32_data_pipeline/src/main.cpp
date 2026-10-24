// The point of this file in its current state is to read the potentiometer voltage
// with the ESP32 and send the measurements over serial communication.

#include <Arduino.h>

// GPIO34 is on ADC1, which keeps working when WiFi is on (ADC2 does not).
const int POT_PIN = 34;

void setup() {
  Serial.begin(9600);
  pinMode(POT_PIN, INPUT);
}

void loop() {
  int rawValue = analogRead(POT_PIN);

  // Uses the ESP32's factory calibration, which corrects for the ADC not
  // being linear. More accurate than rawValue * 3.3 / 4095.
  float voltage = analogReadMilliVolts(POT_PIN) / 1000.0;

  Serial.print(rawValue);
  Serial.print(",");
  Serial.println(voltage, 3);  // 3 decimal places

  delay(200);
}