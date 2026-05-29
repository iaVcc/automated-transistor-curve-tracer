#include <Arduino.h>

const int LED_PIN = 0;
const int POT_PIN = A0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int rawValue = analogRead(POT_PIN);
  float voltage = rawValue * (5/1023.0);

  Serial.print("Raw: ");
  Serial.println(rawValue);
  Serial.print("Voltage: ");
  Serial.println(voltage);

  delay(200);
}