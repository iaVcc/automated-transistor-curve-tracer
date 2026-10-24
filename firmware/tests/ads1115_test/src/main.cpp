// ADS1115 standalone test: prints all four input channels twice a second.
// Wiring: VDD->3V3, GND->GND, SDA->21, SCL->22, ADDR->GND.
// Test inputs: A0->GND (expect 0 V), A1->3V3 (compare with multimeter),
// A2->pot middle pin (compare with multimeter at a few positions).

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 adc;

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!adc.begin(0x48)) {
    Serial.println("ADS1115 not found at 0x48");
    while (true) delay(1000);
  }
  adc.setGain(GAIN_ONE);  // +/-4.096 V range
}

void loop() {
  for (uint8_t ch = 0; ch < 4; ch++) {
    float v = adc.computeVolts(adc.readADC_SingleEnded(ch));
    Serial.printf("A%d=%.4f V  ", ch, v);
  }
  Serial.println();
  delay(500);
}
