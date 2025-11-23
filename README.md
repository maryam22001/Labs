# IoT & Embedded Systems Lab Repository

This repository contains a collection of labs demonstrating various embedded systems and IoT concepts, ranging from low-level AVR motor control to real-time WebSocket communication and secure MQTT messaging.

## Table of Contents

1.  [Module 1: Stepper Motor Control System](https://www.google.com/search?q=%23module-1-stepper-motor-control-system)
2.  [Module 2: Real-time IoT LED Controller (WebSockets)](https://www.google.com/search?q=%23module-2-real-time-iot-led-controller-websockets)
3.  [Module 3: MQTT Sensor Simulation (HiveMQ)](https://www.google.com/search?q=%23module-3-mqtt-sensor-simulation-hivemq)

-----

## Module 1: Stepper Motor Control System

**Location:** `/Motor/sketch_oct11a.ino`

A complete control system for a stepper motor using an AVR/Arduino architecture. It features a Finite State Machine (FSM) to handle various operational modes, speed control, and emergency safety protocols.

### Features

  * **5 Operational Modes:**
    0\.  **STOP:** Motor is idle.
    1.  **MANUAL\_HALF:** Single half-step per button press.
    2.  **MANUAL\_FULL:** Single full-step per button press.
    3.  **AUTO\_HALF:** Continuous rotation using half-stepping.
    4.  **AUTO\_FULL:** Continuous rotation using full-stepping.
  * **Emergency Stop:** Triggered by holding the Start/Stop button for 2 seconds. Immediately halts the system and blinks 'E' on the display.
  * **Visual Interface:**
      * **7-Segment Display:** Shows the current mode ID (0-4) or 'E' for error.
      * **LEDs:** Indicate Status (Running/Stopped) and Direction (Forward/Reverse).
  * **Control Inputs:**
      * **Mode:** Cycles through operation modes.
      * **Start/Stop:** Toggles motor state.
      * **Direction:** Toggles between Clockwise and Counter-Clockwise.
      * **Speed Up/Down:** Adjusts the delay between steps (50ms - 2000ms).

### Hardware Pinout

| Component | Pin | Description |
| :--- | :--- | :--- |
| **Buttons** | 2, 3, 4, 5, 6 | Mode, Start/Stop, Dir, Speed Up, Speed Down |
| **LEDs** | 7, 8 | Status LED, Direction LED |
| **7-Segment** | 9-13, A0, A1 | Segments A-G |
| **Stepper** | A2-A5 | Coils 1-4 |

### Code Highlights

The system uses non-blocking logic (`millis()`) for button debouncing and step timing, allowing the UI to remain responsive while the motor moves.

-----

## Module 2: Real-time IoT LED Controller (WebSockets)

**Location:** `/Task1-iot`

An IoT application that establishes bidirectional communication between a web browser and an ESP8266 (NodeMCU) to control an LED's state and brightness in real-time.

### Architecture

1.  **Node.js Server (`server.js`):**
      * Hosts a web interface (`public/index.html`).
      * Runs a WebSocket server to broadcast JSON commands.
      * Provides a REST API endpoint (`/api/command/:cmd`) for external control.
2.  **Web Client (`index.html`):**
      * A responsive dashboard to toggle the LED and adjust brightness via a slider.
      * Displays a live log of sent/received WebSocket messages.
3.  **Embedded Client (`sketch_oct15a.ino`):**
      * Runs on ESP8266 (NodeMCU).
      * Connects to WiFi and the WebSocket server.
      * Parses JSON commands to control the LED via PWM.

### Communication Protocol

Data is exchanged in JSON format:

  * **Command (Web -\> ESP):** `{"command": "BRIGHTNESS", "value": 128}`
  * **Status (ESP -\> Web):** `{"status": "LED is ON"}`

### Setup & Usage

1.  **Server:** Run `node server.js` to start the server on port 3000.
2.  **Hardware:** Flash the `ino` file to a NodeMCU. *Note: Ensure the `SERVER` IP in the Arduino code matches your PC's local IP address.*
3.  **Control:** Open `http://localhost:3000` to control the LED.

-----

## Module 3: MQTT Sensor Simulation (HiveMQ)

**Location:** `/Task2-iot`

A Python-based simulation of an IoT sensor node that publishes telemetry data securely to a cloud broker (HiveMQ) using the MQTT protocol.

### Components

  * **`psuedoSensor.py`:** A class that generates realistic temperature and humidity data. It iterates through predefined arrays with added random noise to simulate natural environmental fluctuations.
  * **`mqtt_client.py`:** The main entry point using the `paho-mqtt` library.
      * **Protocol:** MQTT v5 over TLS/SSL (Port 8883).
      * **Topic:** Publishes to `pi/sensor`.
      * **Subscription:** Listens to `pi/#` to verify message receipt.

### Usage

The script initializes a secure connection to the HiveMQ cloud instance and enters a loop:

1.  Generates new sensor values (e.g., `{"humidity": 45.2, "temperature": 22.5}`).
2.  Publishes the JSON payload to the topic `pi/sensor`.
3.  Waits for 10 seconds before the next reading.

### Dependencies

  * Python 3.x
  * `paho-mqtt`

<!-- end list -->

```bash
pip install paho-mqtt
python mqtt_client.py
```
