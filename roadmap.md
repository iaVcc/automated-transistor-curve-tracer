# Roadmap

This file tracks the major development phases for the automated transistor curve tracer project.

The plan is to build and verify the system one block at a time before combining everything into a full measurement platform.



## Phase 1: ESP32 Embedded Data Acquisition Pipeline

Goal: Establish the basic firmware and software workflow needed for automated measurements using the ESP32.

* [x] Set up ESP32 PlatformIO environment
* [x] Validate GPIO output with external LED
* [ ] Validate 3.3V-safe analog input measurement
* [x] Validate serial communication between ESP32 and computer
* [ ] Set up Python environment
* [ ] Read serial data from ESP32 using Python
* [ ] Plot live measurement data using Python


## Phase 2: DAC and ADC Validation

Goal: Verify the external DAC and ADC modules before using them in the curve tracing circuit.

* [ ] Interface MCP4725 DAC with ESP32 over I2C
* [ ] Generate fixed DAC output voltages
* [ ] Generate controlled voltage sweeps
* [ ] Verify DAC output using multimeter
* [ ] Interface ADS1115 ADC with ESP32 over I2C
* [ ] Measure known voltages using ADS1115
* [ ] Compare ADC measurements against multimeter readings
* [ ] Send ADC data to Python for plotting and logging



## Phase 3: Diode I-V Characterization

Goal: Build the first working curve tracing circuit using diodes as the device under test.

* [ ] Build current-limited diode test circuit
* [ ] Measure diode voltage and current
* [ ] Generate first measured I-V curve
* [ ] Characterize 1N4148 switching diode
* [ ] Characterize 1N4007 rectifier diode
* [ ] Characterize 1N5819 Schottky diode
* [ ] Compare measured diode curves
* [ ] Save plots and measurement data



## Phase 4: BJT Characterization

Goal: Extend the measurement setup to characterize small-signal BJTs.

* [ ] Design safe BJT test circuit
* [ ] Build initial 2N3904 test setup
* [ ] Control base drive
* [ ] Sweep collector voltage
* [ ] Measure collector current
* [ ] Generate first BJT output curve
* [ ] Compare 2N3904 and 2N2222 behavior
* [ ] Document measurement limitations and sources of error



## Phase 5: MOSFET Characterization

Goal: Add support for basic MOSFET transfer/output measurements.

* [ ] Design safe MOSFET test circuit
* [ ] Build initial 2N7000 test setup
* [ ] Sweep gate voltage
* [ ] Measure drain current
* [ ] Generate MOSFET transfer curve
* [ ] Test low-current power MOSFET behavior
* [ ] Document threshold voltage behavior and limitations



## Phase 6: Calibration and Measurement Improvements

Goal: Improve measurement quality and understand system accuracy.

* [ ] Add precision sense resistors
* [ ] Compare system readings against multimeter measurements
* [ ] Add software calibration constants
* [ ] Estimate measurement error
* [ ] Investigate noise and grounding issues
* [ ] Capture oscilloscope measurements
* [ ] Add basic firmware protection limits



## Phase 7: PCB Design

Goal: Convert the validated breadboard prototype into a cleaner PCB-based design.

* [ ] Create system block diagram
* [ ] Create KiCad schematic
* [ ] Choose connectors and DUT socket layout
* [ ] Create PCB layout
* [ ] Add ground plane
* [ ] Run design rule checks
* [ ] Export Gerber files
* [ ] Order PCB
* [ ] Assemble PCB
* [ ] Perform PCB bring-up
* [ ] Compare PCB performance against breadboard prototype



## Phase 8: Final Documentation

Goal: Clean up the project documentation so the design, measurements, and results are easy to follow.


* [ ] Update README with project overview
* [ ] Add system block diagram
* [ ] Add circuit schematics
* [ ] Add prototype photos
* [ ] Add oscilloscope captures
* [ ] Add measured plots
* [ ] Explain measurement method
* [ ] Document limitations and future improvements
