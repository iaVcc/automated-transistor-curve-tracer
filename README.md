# Automated Transistor Curve Tracer and Component Qualifier

## Project Overview
This project is a custom mixed-signal hardware-in-the-loop (HIL) testing platform designed to automate the electrical characterization and qualification of discrete semiconductors (BJTs, MOSFETs, and Diodes).

Ever since first learning about semiconductor physics and transistor operation in my ELEC courses, I've been highly curious about how these devices behave in the real world versus their idealized textbook equations. This platform was born out of a desire to dive deeper into that curiosity by building a physical system capable of capturing actual component behaviour. Bridging the gap between analog hardware design and automated software data collection allows me to apply classroom textbook theory directly to a physical system in a way that is deeply engaging to me.


## Project Development Phases

### Phase 1: Simulation and Breadboard Proof of Concept
* Validate core analog sensing circuits in LTspice to ensure proper voltage scaling.
* Assemble a preliminary breadboard prototype to verify stable communication between the Arduino and external DAC.
* Confirm basic analog-to-digital telemetry acquisition.

### Phase 2: Schematic Capture and PCB Design (KiCad 10)
* Translate the verified breadboard circuit into an official schematic within KiCad 10.
* Layout and route a custom dual-layer PCB, prioritizing ground planes to minimize signal noise.
* Organize clean footprint placement for the test socket, instrumentation amplifiers, and terminal blocks.

### Phase 3: Hardware Assembly and Power Bring-Up
* Source components and perform hands-on soldering of the manufactured PCB.
* Conduct a formal hardware bring-up process using a multimeter to verify power rail thresholds.
* Insulate and secure the platform to ensure a safe debugging environment.

### Phase 4: Firmware, Software Integration and Calibration
* Write the embedded C++ firmware for the Arduino to execute precision voltage sweeps.
* Develop the Python automation script to handle real-time serial parsing and plot characteristic curves using Matplotlib.
* Calibrate system measurements against a benchmark multimeter to ensure data accuracy.


## Complete Hardware and Components Checklist

### Lab Equipment & Prototyping Gear
* [x] Variable DC Bench Power Supply (Ideally with adjustable current limiting)
* [ ] Digital Oscilloscope (Minimum 2-channel, for signal integrity and noise analysis)
* [ ] Digital Multimeter (DMM) with test leads
* [ ] Solderless Breadboard (830 tie-point)
* [ ] Solid-core Male-to-Male jumper wires
* [ ] Stranded Male-to-Female jumper wires
* [ ] USB Data Cable (compatible with chosen Arduino)

### Processing & Integrated Circuits (ICs)
* [ ] Arduino Uno (R3/R4) or Arduino Nano
* [ ] MCP4725 I2C DAC Breakout Module
* [ ] MCP6022 or AD822 Rail-to-Rail Op-Amp IC (DIP-8 package)

### Resistors (Sensing & Bias)
* [ ] 10 Ω resistor (0.1% or 1% tolerance)
* [ ] 100 Ω resistor (0.1% or 1% tolerance)
* [ ] 1 kΩ resistor (0.1% or 1% tolerance)
* [ ] 10 kΩ resistors (Standard 5% tolerance)
* [ ] 100 kΩ resistors (Standard 5% tolerance)

### Capacitors (Power Rail Decoupling)
* [ ] 0.1 µF Ceramic Capacitors (100nF / Code 104)
* [ ] 10 µF Electrolytic Capacitors (Rated 25V or higher)

### Hardware, Connectors & PCB
* [ ] 14-Pin or 16-Pin ZIF (Zero Insertion Force) Socket
* [ ] 2-Pin Screw Terminal Blocks (5.08mm pitch)
* [ ] 3-Pin Screw Terminal Blocks (5.08mm pitch)
* [ ] Male Breakaway Pin Headers (0.1" / 2.54mm pitch)
* [ ] Female Pin Headers (0.1" / 2.54mm pitch)
* [ ] Custom Manufactured Dual-Layer PCB (KiCad export)

### Semiconductors Under Test (DUTs)
* [ ] 2N2222 NPN Transistors
* [ ] BC547 NPN Transistors
* [ ] 2N3906 PNP Transistors
* [ ] BC557 PNP Transistors
* [ ] BS170 N-Channel MOSFETs
* [ ] IRF540N N-Channel MOSFETs
* [ ] 1N4148 Switching Diodes
* [ ] 1N4007 Rectifier Diodes
* [ ] 1N4733A 5.1V Zener Diodes

### Assembly Tools & Consumables
* [ ] Temperature-controlled Soldering Iron & Stand
* [ ] Solder wire (60/40 Rosin Core or Lead-Free)
* [ ] Soldering Flux (pen or paste)
* [ ] Desoldering pump (Solder sucker) or Desoldering wick
* [ ] Flush cutters (Wire snips)
