// I2C scanner: lists the address of every chip that answers on the bus.
// First thing to run whenever an I2C chip isnt responding.
// Expected: 0x48 (ADS1115) and 0x60 (MCP4725). Each DAC sits on its own bus later, so both stay 0x60.

#include <Arduino.h>
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();  // SDA=21, SCL=22
}

void loop() {
  Serial.println("Scanning...");
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    // endTransmission() returns 0 if a chip acknowledged this address
    if (Wire.endTransmission() == 0) {
      Serial.printf("Found device at 0x%02X\n", addr);
    }
  }
  delay(2000);
}
