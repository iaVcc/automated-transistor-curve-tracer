// MCP4725 standalone test: steps the output through five levels, 4 s each,
// so OUT can be checked against a multimeter.
// Wiring: VCC->3V3, GND->GND, SDA->21, SCL->22. Multimeter on OUT and GND.

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

// Factory address. Both DACs use 0x60, test them one at a time.
const uint8_t DAC_ADDR = 0x60;

Adafruit_MCP4725 dac;
const uint16_t levels[] = {0, 1024, 2048, 3072, 4095};

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!dac.begin(DAC_ADDR)) {
    Serial.printf("MCP4725 not found at 0x%02X\n", DAC_ADDR);
    while (true) delay(1000);
  }
}

void loop() {
  for (uint16_t code : levels) {
    dac.setVoltage(code, false);
    Serial.printf("code %4u -> expect about %.3f V\n", code, code * 3.3 / 4096);
    delay(4000);  // time to read the multimeter
  }
}
