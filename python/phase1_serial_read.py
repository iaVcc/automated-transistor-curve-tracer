import serial
import time

PORT = "COM3"
BAUD_RATE = 9600

def main():
    with serial.Serial(PORT, BAUD_RATE, timeout=1) as arduino:
        time.sleep(2)

        while True:
            line = arduino.readline().decode("utf-8").strip()

            if not line:
                continue

            try:
                raw_text, voltage_text = line.split(",")
                raw_value = int(raw_text)
                voltage = float(voltage_text)

                print(f"Raw: {raw_value}, Voltage: {voltage:.2f} V")
            
            except ValueError:
                print(f"Could not parse line: {line}")

if __name__ == "__main__":
    main()