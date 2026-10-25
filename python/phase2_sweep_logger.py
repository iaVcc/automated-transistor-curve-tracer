"""Phase 2: run a DAC sweep on the ESP32, save it to CSV, and plot the accuracy.

Usage (with the serial monitor closed, since only one program can own the port):
    python python/phase2_sweep_logger.py --port COM3

It sends "s" to the ESP32, collects every CSV row between "# SWEEP_START" and
"# SWEEP_END", writes them to a timestamped file, then plots:
    top:    measured voltage vs the voltage the DAC was told to output
    bottom: the error between them, in millivolts
"""

import argparse
import csv
import time
from datetime import datetime
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import serial  # this is the pyserial package


def run_sweep(port: str, baud: int) -> list[tuple[int, float, float]]:
    rows = []

    # "with" closes the port automatically, even if something crashes.
    with serial.Serial(port, baud, timeout=2) as ser:
        # Opening the port resets most ESP32 boards. Wait for it to boot,
        # then throw away the startup text.
        time.sleep(2)
        ser.reset_input_buffer()

        ser.write(b"s\n")

        in_sweep = False
        while True:
            raw = ser.readline()
            if not raw:
                raise TimeoutError("No data from the ESP32. Is the right firmware flashed?")

            # errors="replace" stops one garbled byte from crashing the script.
            line = raw.decode(errors="replace").strip()

            if line == "# SWEEP_START":
                in_sweep = True
                continue
            if line == "# SWEEP_END":
                break
            if not in_sweep or line.startswith("code"):
                continue  # skip anything before the data and the header row

            code, set_v, measured_v = line.split(",")
            rows.append((int(code), float(set_v), float(measured_v)))

    return rows


def save_csv(rows, out_dir: Path) -> Path:
    out_dir.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    path = out_dir / f"phase2_sweep_{stamp}.csv"

    with open(path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["code", "set_v", "measured_v"])
        writer.writerows(rows)

    return path


def plot(rows, title: str):
    # np.array turns the list of tuples into a 2D array; .T flips it so each
    # column (code, set, measured) becomes its own 1D array we can unpack.
    codes, set_v, measured_v = np.array(rows).T
    error_mv = (measured_v - set_v) * 1000

    fig, (ax_top, ax_err) = plt.subplots(2, 1, sharex=True, figsize=(8, 6))

    ax_top.plot(set_v, measured_v, "o-", markersize=3, label="Measured")
    ax_top.plot(set_v, set_v, "--", color="gray", label="Ideal")
    ax_top.set_ylabel("ADC measured (V)")
    ax_top.set_title(title)
    ax_top.legend()
    ax_top.grid(True)

    ax_err.plot(set_v, error_mv, "o-", markersize=3, color="tab:red")
    ax_err.axhline(0, color="gray", linewidth=0.8)
    ax_err.set_xlabel("DAC set voltage (V)")
    ax_err.set_ylabel("Error (mV)")
    ax_err.grid(True)

    print(f"Points: {len(rows)}")
    print(f"Mean error: {error_mv.mean():+.1f} mV   Max |error|: {np.abs(error_mv).max():.1f} mV")

    fig.tight_layout()
    plt.show()


def main():
    parser = argparse.ArgumentParser(description="Phase 2 DAC/ADC sweep logger")
    parser.add_argument("--port", default="COM3")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--out", type=Path, default=Path("data"))
    args = parser.parse_args()

    rows = run_sweep(args.port, args.baud)
    path = save_csv(rows, args.out)
    print(f"Saved {path}")
    plot(rows, f"MCP4725 -> ADS1115 sweep ({path.name})")


if __name__ == "__main__":
    main()