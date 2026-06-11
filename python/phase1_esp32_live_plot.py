import serial
import time
import matplotlib.pyplot as plt
from collections import deque

PORT = "COM4"
BAUD_RATE = 9600
MAX_POINTS = 50

times = deque(maxlen=MAX_POINTS)
voltages = deque(maxlen=MAX_POINTS)

def main():
    start_time = time.time()

    with serial.Serial(PORT, BAUD_RATE, timeout=1) as esp32:
        time.sleep(2)

        plt.ion()
        fig, ax = plt.subplots()

        line_plot, = ax.plot([], [])

        ax.set_title("Live ESP32 Voltage Reading")
        ax.set_xlabel("Time (s)")
        ax.set_ylabel("Voltage (V)")
        ax.set_ylim(0, 3.5)

        while True:
            line = esp32.readline().decode("utf-8", errors="ignore").strip()

            if not line:
                continue

            try:
                raw_text, voltage_text = line.split(",")
                voltage = float(voltage_text)

                current_time = time.time() - start_time

                times.append(current_time)
                voltages.append(voltage)

                line_plot.set_data(times, voltages)

                ax.relim()
                ax.autoscale_view(scalex=True, scaley=False)

                plt.pause(0.01)

            except ValueError:
                print(f"Could not parse line: {line}")

if __name__ == "__main__":
    main()