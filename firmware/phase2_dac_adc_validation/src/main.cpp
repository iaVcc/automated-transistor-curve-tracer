// Phase 2: drive the MCP4725 DACs and read the ADS1115 ADC over I2C.
//
// What this proves:
//   1. The ESP32 can set a voltage with each DAC.
//   2. The ADS1115 measures that voltage accurately.
//   3. A sweep (DAC steps up, ADC reads each step) works end to end. A curve
//      tracer is this same loop with a transistor in the middle.
//
// Wiring (everything powered from the ESP32's 3.3V pin for now):
//   ESP32 3V3 -> VCC/VDD on all three modules
//   ESP32 GND -> GND on all three modules
//   Bus 1: ESP32 GPIO21 -> SDA, GPIO22 -> SCL on the ADS1115 and DAC A
//   Bus 2: ESP32 GPIO32 -> SDA, GPIO33 -> SCL on DAC B only
//   Both DACs keep their factory address 0x60. Because they sit on separate
//   I2C buses, the ESP32 can still talk to each one on its own, with no
//   solder jumper needed.
//   DAC A OUT -> ADS1115 A0
//   DAC B OUT -> ADS1115 A1
//   ADS1115 ADDR pin -> GND (address 0x48)
//
// Why 3.3V and not 5V: the modules have pull-up resistors on SDA/SCL that pull
// up to whatever VCC is. At 5V those would push 5V into the ESP32's pins, and
// the ESP32 is only rated for 3.3V. Later the board will handle this properly.
//
// Serial commands (type in the PlatformIO serial monitor, press Enter):
//   s          sweep DAC A from 0 V to full scale, print a CSV row per step
//   a 1.25     set DAC A to 1.25 V and hold it (check it with your multimeter)
//   b 0.80     set DAC B to 0.80 V and hold it
//   r          read all four ADC channels once
//   h          print this help

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1X15.h>

// Measure the ESP32's 3V3 pin with your multimeter and put the real value
// here. The DAC's output is a fraction of its supply, so this sets accuracy.
const float DAC_SUPPLY_V = 3.32;  // measured on my board

// The MCP4725 is 12-bit: codes 0..4095. 4096 steps across the supply voltage.
const uint16_t DAC_MAX_CODE = 4095;

const uint8_t DAC_A_ADDR = 0x60;
const uint8_t DAC_B_ADDR = 0x60;  // same address, but on the second bus
const uint8_t ADC_ADDR = 0x48;

// Pins for the second I2C bus (DAC B). Any free GPIOs work; 32/33 have no
// special boot-time role, so they're safe.
const int BUS2_SDA = 32;
const int BUS2_SCL = 33;

// Sweep settings: 64 codes per step gives 65 points, about 50 mV apart.
const uint16_t SWEEP_STEP = 64;
// Time to let the DAC output settle before measuring. The DAC itself settles
// in microseconds; this mostly gives the ADC a clean conversion window.
const uint16_t SETTLE_MS = 5;

// These are objects from the Adafruit libraries. Each one represents one chip
// on the I2C bus and hides the raw register reads/writes behind methods.
Adafruit_MCP4725 dacA;
Adafruit_MCP4725 dacB;
Adafruit_ADS1115 adc;

// Convert a voltage you want into the 12-bit code the DAC understands.
// The clamp means a typo like "a 9" can't ask for an impossible code.
uint16_t voltsToCode(float volts) {
  float code = volts / DAC_SUPPLY_V * (DAC_MAX_CODE + 1);
  if (code < 0) code = 0;
  if (code > DAC_MAX_CODE) code = DAC_MAX_CODE;
  return (uint16_t)code;
}

float codeToVolts(uint16_t code) {
  return code * DAC_SUPPLY_V / (DAC_MAX_CODE + 1);
}

// Read one ADS1115 channel and return volts.
// readADC_SingleEnded() returns a signed 16-bit count; computeVolts() scales it
// using the gain setting chosen in setup().
float readVolts(uint8_t channel) {
  int16_t counts = adc.readADC_SingleEnded(channel);
  return adc.computeVolts(counts);
}

void printHelp() {
  Serial.println(F("Commands: s = sweep DAC A | a <V> = set DAC A | b <V> = set DAC B | r = read ADC | h = help"));
}

void runSweep() {
  // The header line lets the Python script know where the data starts.
  Serial.println(F("# SWEEP_START"));
  Serial.println(F("code,set_v,measured_v"));

  // Loop over codes. The extra "last" step makes sure the sweep ends exactly at
  // 4095 even though 4095 isn't a multiple of 64.
  for (uint32_t code = 0; code <= DAC_MAX_CODE + SWEEP_STEP; code += SWEEP_STEP) {
    uint16_t c = (uint16_t)min(code, (uint32_t)DAC_MAX_CODE);

    // false = don't save to the DAC's EEPROM. EEPROM wears out after ~1M
    // writes, so only write it when you want a power-on default.
    dacA.setVoltage(c, false);
    delay(SETTLE_MS);

    float measured = readVolts(0);

    Serial.print(c);
    Serial.print(',');
    Serial.print(codeToVolts(c), 4);  // 4 = digits after the decimal point
    Serial.print(',');
    Serial.println(measured, 4);

    if (c == DAC_MAX_CODE) break;
  }

  dacA.setVoltage(0, false);  // leave the output at 0 V when done
  Serial.println(F("# SWEEP_END"));
}

void setAndHold(Adafruit_MCP4725 &dac, uint8_t adcChannel, float volts, char name) {
  uint16_t code = voltsToCode(volts);
  dac.setVoltage(code, false);
  delay(SETTLE_MS);

  Serial.print(F("DAC "));
  Serial.print(name);
  Serial.print(F(" code="));
  Serial.print(code);
  Serial.print(F(" set="));
  Serial.print(codeToVolts(code), 4);
  Serial.print(F(" V  ADC reads="));
  Serial.print(readVolts(adcChannel), 4);
  Serial.println(F(" V  (now check with your multimeter)"));
}

void readAll() {
  for (uint8_t ch = 0; ch < 4; ch++) {
    Serial.print(F("A"));
    Serial.print(ch);
    Serial.print(F("="));
    Serial.print(readVolts(ch), 4);
    Serial.print(ch < 3 ? F(" V  ") : F(" V\n"));
  }
}

// Stop forever with a message. Used when a chip doesn't answer on the bus,
// which almost always means a wiring problem or the wrong address.
void halt(const __FlashStringHelper *msg) {
  Serial.println(msg);
  while (true) delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(500);  // give the serial monitor a moment to connect

  // Bus 1 on the ESP32's default pins (SDA=21, SCL=22): ADS1115 + DAC A.
  Wire.begin();
  // The ESP32 has two I2C controllers. Wire1 is the second one, started on
  // our own choice of pins.
  Wire1.begin(BUS2_SDA, BUS2_SCL);

  // begin() returns false if no chip acknowledges at that address.
  if (!dacA.begin(DAC_A_ADDR)) halt(F("DAC A not found at 0x60. Check SDA/SCL/power."));
  // Passing &Wire1 tells the library to use the second bus for this DAC.
  if (!dacB.begin(DAC_B_ADDR, &Wire1)) halt(F("DAC B not found on bus 2. Check GPIO32 (SDA) / GPIO33 (SCL)."));
  if (!adc.begin(ADC_ADDR)) halt(F("ADS1115 not found at 0x48. Check ADDR is tied to GND."));

  // Gain sets the ADC's full-scale range. GAIN_ONE = +/-4.096 V, which covers
  // 0 to 3.3 V. The input must still never go above the ADS1115's own VDD.
  adc.setGain(GAIN_ONE);

  dacA.setVoltage(0, false);
  dacB.setVoltage(0, false);

  Serial.println(F("Phase 2 DAC/ADC test ready."));
  printHelp();
}

// Run one complete command line, e.g. "a 1.25".
void handleCommand(String line) {
  line.trim();  // removes stray spaces
  if (line.length() == 0) return;

  char cmd = line.charAt(0);
  // Everything after the command letter is the argument, e.g. "1.25".
  float arg = line.substring(1).toFloat();

  switch (cmd) {
    case 's': runSweep(); break;
    case 'a': setAndHold(dacA, 0, arg, 'A'); break;
    case 'b': setAndHold(dacB, 1, arg, 'B'); break;
    case 'r': readAll(); break;
    case 'h': printHelp(); break;
    default:
      Serial.print(F("Unknown command: "));
      Serial.println(line);
      printHelp();
  }
}

void loop() {
  // The serial monitor sends each key the moment you press it, so collect
  // characters here until Enter arrives, then run the whole line at once.
  // "static" keeps the text between calls to loop().
  static String buffer;

  while (Serial.available()) {
    char ch = Serial.read();

    if (ch == '\r' || ch == '\n') {
      // Enter can send \r, \n or both; an empty buffer means we already ran it.
      if (buffer.length() > 0) {
        Serial.println();
        handleCommand(buffer);
        buffer = "";
      }
    } else if (ch == '\b' || ch == 127) {
      // Backspace: drop the last character and erase it on screen.
      if (buffer.length() > 0) {
        buffer.remove(buffer.length() - 1);
        Serial.print(F("\b \b"));
      }
    } else {
      buffer += ch;
      Serial.print(ch);  // echo it so you can see what you're typing
    }
  }
}