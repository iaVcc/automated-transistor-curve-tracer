#include <Arduino.h>

const int LED_PIN = 8;
int counter = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
  delay(250);

  Serial.print("Counter: ");
  Serial.println(counter);

  counter++;
}