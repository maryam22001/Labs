# Embedded Systems & IoT Repository

This repository contains a comprehensive collection of labs demonstrating computing concepts across multiple abstraction layers. The projects range from **MIPS Assembly language** processing and **AVR motor control** to **Real-time WebSockets** and **Secure MQTT** cloud messaging.

## Table of Contents
1. [Module 1: Stepper Motor Control System](#module-1-stepper-motor-control-system)
2. [Module 2: Real-time IoT LED Controller (WebSockets)](#module-2-real-time-iot-led-controller-websockets)
3. [Module 3: MQTT Sensor Simulation (HiveMQ)](#module-3-mqtt-sensor-simulation-hivemq)
4. [Module 4: MIPS Assembly Language Programming](#module-4-mips-assembly-language-programming)

---

## Module 1: Stepper Motor Control System
**Location:** `/Motor/sketch_oct11a.ino`

A complete control system for a stepper motor using an AVR/Arduino architecture. It features a Finite State Machine (FSM) to handle various operational modes, speed control, and emergency safety protocols.

### Features
* **5 Operational Modes:** STOP, MANUAL (Half/Full), and AUTO (Half/Full).
* **Emergency Stop:** Triggered by holding the Start/Stop button for 2 seconds.
* **Visual Interface:** 7-Segment Display (showing Mode ID or 'E' for error) and Status LEDs.
* **Control Inputs:** Buttons for Mode, Start/Stop, Direction, and Speed adjustments (50ms - 2000ms).

---

## Module 2: Real-time IoT LED Controller (WebSockets)
**Location:** `/Task1-iot`

An IoT application establishing bidirectional communication between a web browser and an ESP8266 (NodeMCU) to control hardware in real-time.

### Architecture
* **Node.js Server:** Hosting a dashboard and WebSocket server.
* **Web Client:** Responsive UI with sliders and status logging.
* **Embedded Client:** ESP8266 parsing JSON commands to control PWM brightness.

### Protocol
JSON payloads are used for state management:
* **Command:** `{"command": "BRIGHTNESS", "value": 128}`
* **Status:** `{"status": "LED is ON"}`

---

## Module 3: MQTT Sensor Simulation (HiveMQ)
**Location:** `/Task2-iot`

A Python-based simulation of an IoT sensor node that publishes telemetry data securely to a cloud broker (HiveMQ) using MQTT v5 over TLS.

### Components
* **`psuedoSensor.py`:** Generates realistic temperature/humidity data with random noise.
* **`mqtt_client.py`:** Publishes to `pi/sensor` and subscribes to `pi/#` for verification.

---

## Module 4: MIPS Assembly Language Programming
**Location:** Root Directory (`Q1.asm`, `Q2-sol.asm`, `assembly.docx`)

This module explores low-level memory management, arithmetic logic, and coprocessor operations using MIPS Assembly.

### Task 1: Integer Parity Checker & Debugging
**File:** `Q1.asm`
A program that accepts an integer input and determines if it is **Even** or **Odd** using bitwise operations.
* **Logic:** Uses `andi $t0, $v0, 1` to isolate the Least Significant Bit (LSB). If LSB is 0, the number is even.
* **Debugging:** The development process involved tracing specific errors such as missing `.data` directives, incorrect syscall arguments (`li $a0, 4` vs `li $v0, 4`), and logic flow issues.

### Task 2: Advanced Array Statistics (Min, Max, Avg)
**File:** `Q2-sol.asm`
A complex program that processes an array of integers (up to 30 elements) provided by the user.
* **Features:**
    * **Dynamic Input:** Stops reading when the user enters a negative number or reaches the 30-element limit.
    * **Single-Pass Logic:** Calculates Min and Max values in a single loop traversal for efficiency.
    * **Floating Point Arithmetic:** Calculates the average using Coprocessor 1 (FPU). It converts the integer sum to single-precision float (`cvt.s.w`) before dividing by 2.0 (`div.s`).
