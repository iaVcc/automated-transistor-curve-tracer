#include <Arduino.h>

const int LED_PIN = 8;

void setup(){
  pinMode(LED_PIN, HIGH);
}

void loop(){
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  digitalWrite(LED_PIN, HIGH);
}