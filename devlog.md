# Devlog

### 05-23-2026
Settled on the automated transistor curve tracer as my main summer project idea, and created the repo.

The goal is to build something that combines analog circuits, embedded programming, measurement, PCB design, and Python-based plotting.



### 05-28-2026
Today I set up the initial GitHub repository for this project. Wrote `tools.md` to track what ima need for this. Gotta write out the roadmap next and get this project started.



### 05-29-2026
Roadmap complete (`roadmap.md`).

Started Phase 1: 
- verified PlatformIO works by wiring external LED blinking with 220Ω resistor on pin 8
- verified that reading analog input works
- verified that the serial monitor output works. 



### 05-30-2026
Got Python reading serial data from the Arduino. The Arduino sends raw ADC values and calculated voltage from a potentiometer, and the Python script parses these and prints the readings.

Finished the live Python plotting from Arduino serial data. Saved plots/pics in `docs/`, this completes phase 1!



### 05-31-2026

Took time to organize the electronic components into a binder system so parts are easier to find during prototyping. Started sorting resistors, capacitors, diodes, transistors, ICs, and modules into labeled sections.

This should make future breadboard testing and debugging less chaotic.


### 06-08-2026

Took way longer than I expected but now every electronic component I own is labeled and categorized. This should make prototyping and finding parts much easier going forward.


### 06-10-2026

Decided to change the main controller from Arduino Uno to ESP32 and restart phase 1, removed old Arduino files. Reason for switching 


### 06-11-2026

Redid Phase 1 using the ESP32. Verified GPIO output, serial communication, 3.3V safe analog input, Python serial reading, and live voltage plotting. Saved the ESP32 potentiometer setup photo and live plot demo in `docs/`.


### 09-25-2026

Returning after a while so I did some Phase 1 cleanup and started Phase 2.