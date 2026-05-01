# Dehumidifier Controller with DHT22

This project is an automated dehumidifier controller built with Arduino. It monitors environmental humidity using a DHT22 sensor and controls a relay to toggle a dehumidifier. It features a manual override button and a hysteresis-based logic to prevent the relay from "flickering" near the setpoint.

## Features

*   **Automatic Control**: Automatically starts the dehumidifier when humidity is too high.
*   **Hysteresis Logic**: Uses a threshold (±2%) to ensure the device doesn't cycle on and off too rapidly.
*   **Manual Override**: A physical button allows you to manually toggle the state and pause automatic measurements for 20 minutes.
*   **Hard Reset**: Long-pressing the button (2 seconds) resets the system to its default state.

---

## Hardware Components

*   **Microcontroller**: Arduino (Uno, Nano, or similar)
*   **Sensor**: DHT22 (AM2302) Temperature & Humidity sensor
*   **Output**: 5V Relay Module (to control the dehumidifier power)
*   **Indicator**: LED (status light)
*   **Input**: Momentary Push Button
*   **Resistors**: 10k Ohm (for DHT22 if not using a breakout board)

---

## Schematic & Wiring

| Component | Arduino Pin | Notes |
| :--- | :--- | :--- |
| **DHT22 Data** | Pin 11 | Requires a pull-up resistor if not a module |
| **LED (+) ** | Pin 12 | Use a 220-330 Ohm resistor |
| **Push Button** | Pin 9 | Connects to GND (Internal Pull-up used) |
| **Relay Signal** | Pin 6 | Controls the dehumidifier power |

---

## How It Works

### 1. Automatic Mode
The system checks the humidity every 5 seconds. 
*   **ON**: Triggered if humidity $\ge 72\%$ (Limit 70 + Threshold 2).
*   **OFF**: Triggered if humidity $\le 68\%$ (Limit 70 - Threshold 2).

### 2. Manual Toggle
*   **Short Press**: Toggles the dehumidifier ON/OFF regardless of current humidity. This also sets a **20-minute delay** before the next automatic measurement, allowing the manual setting to persist.
*   **Long Press (2s)**: The LED will blink. Releasing the button will reset the timer to 5 seconds and return the system to automatic logic.

---

## Installation

1.  **Libraries**: Install the [DHT-Sensors-Non-Blocking (By  Toan Nguyen)](https://github.com/toannv17/DHT-Sensors-Non-Blocking) library via the Arduino Library Manager.
2.  **Configuration**: Adjust the `HUMIDITY_LIMIT` constant in the code if you prefer a different target (e.g., 60%).
3.  **Upload**: Flash the code to your Arduino.
4.  **Monitor**: Open the Serial Monitor at **9600 baud** to see real-time temperature and humidity data. library via the Arduino Library Manager.
