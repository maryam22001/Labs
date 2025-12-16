# Embedded Systems & IoT Repository

This repository contains a comprehensive collection of labs demonstrating computing concepts across multiple abstraction layers. The projects range from **MIPS Assembly language** processing and **AVR motor control** to **Real-time WebSockets** and **Secure MQTT** cloud messaging.

## Table of Contents
1. [Module 1: Stepper Motor Control System](#module-1-stepper-motor-control-system)
2. [Module 2: Real-time IoT LED Controller (WebSockets)](#module-2-real-time-iot-led-controller-websockets)
3. [Module 3: MQTT Sensor Simulation (HiveMQ)](#module-3-mqtt-sensor-simulation-hivemq)
4. [Module 4: MIPS Assembly Language Programming](#module-4-mips-assembly-language-programming)
5. [Module 5: AVR Timers & Interrupts System](#module-5-avr-timers--interrupts-system)
6. [Module 6: CPUSIM Processor Design & Simulation](#module-6-cpusim-processor-design--simulation)

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

---

## Module 5: AVR Timers & Interrupts System
**Location:** `/AVR-task2/sketch_dec3a/sketch_dec3a.ino`
**Simulation (SimulIDE):** `/AVR-task2/backup.sim1`

A practical implementation demonstrating the simultaneous use of all three ATmega328P timers (Timer0, Timer1, Timer2) to manage concurrent tasks without blocking the main loop.

### Architecture & Timer Assignments
* **Timer0 (CTC Mode):**
    * **Function:** Controls the blinking frequency of an LED on **PB0**.
    * **Interaction:** Pressing **Button 1 (PD2)** cycles through 3 different blink speeds by updating the `OCR0A` compare match threshold.
* **Timer1 (Fast PWM Mode):**
    * **Function:** Controls the brightness of an LED on **PB1** using 8-bit Fast PWM.
    * **Interaction:** Pressing **Button 2 (PD3)** increases the duty cycle (`OCR1A`), cycling through 5 brightness levels.
* **Timer2 (Normal Mode):**
    * **Function:** Acts as a dedicated hardware timer for button debouncing.
    * **Logic:** Generates a delay (approx. 15 overflows) to ignore noise/bouncing after a button press is detected.

---

## Module 6: CPUSIM Processor Design & Simulation
**Location:** `/CPUSIM`
**Documentation:** `/CPUSIM/CPU SIM TASK.pdf`

This module involves the design, microcoding, and simulation of a custom 16-bit CPU architecture to explore the trade-offs between RISC and CISC design philosophies.

### Hardware Design
* **Registers:** Custom set including Accumulator (ACC), Instruction Register (IR), Program Counter (PC), and General Purpose Registers (R0, R1).
* **Instruction Set:** Implemented a custom ISA including `load`, `store`, `add`, `multiply`, `jump`, and conditional branches.

### Architectural Comparison Experiment
The project implements multiplication using two distinct approaches to demonstrate hardware vs. software complexity:

1.  **Software Multiplication (RISC Approach):**
    * **Method:** Repeated addition using a loop in assembly (Code 1).
    * **Characteristics:** High program complexity, higher memory usage, but simpler hardware instruction set.

2.  **Hardware Multiplication (CISC Approach):**
    * **Method:** A dedicated `multiply` instruction microcoded directly into the CPU (Code 2).
    * **Characteristics:** Low program complexity (single instruction), efficient memory usage, but requires complex internal hardware logic.
