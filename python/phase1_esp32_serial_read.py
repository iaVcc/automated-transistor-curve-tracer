# The point of 'phase1_esp32_live_plot.py' is to recieve and parse those
# serial measurements on the computer.

import serial
import time

PORT = "COM3"
BAUD_RATE = 9600

def main():
    with serial.Serial(PORT, BAUD_RATE, timeout=1) as esp32:
        time.sleep(2)

        while True:
            # errors="ignore" skips garbage bytes the ESP32 can send when booting.
            line = esp32.readline().decode("utf-8", errors="ignore").strip()

            if not line:
                continue

            try:
                raw_text, voltage_text = line.split(",")
                raw_value = int(raw_text)
                voltage = float(voltage_text)

                print(f"Raw: {raw_value}, Voltage: {voltage:.3f} V")

            except ValueError:
                print(f"Could not parse line: {line}")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Stopped")