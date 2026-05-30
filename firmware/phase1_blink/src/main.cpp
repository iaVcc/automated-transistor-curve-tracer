#include <Arduino.h>

const int POT_PIN = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int rawValue = analogRead(POT_PIN);
  float voltage = rawValue * (5/1023.0);

  Serial.print(rawValue);
  Serial.print(",");
  Serial.println(voltage);

  delay(200);
}