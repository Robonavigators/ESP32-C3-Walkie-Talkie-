# ESP32-C3 Encrypted Walkie-Talkie Network

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status: Active](https://img.shields.io/badge/Status-Active-brightgreen.svg)
![Hardware: ESP32-C3](https://img.shields.io/badge/Hardware-ESP32--C3%20Pro%20Mini-blue.svg)
![Audio: I2S](https://img.shields.io/badge/Audio-I2S%20Digital-orange.svg)

A low-latency, encrypted, multi-device walkie-talkie system built on the ESP32-C3 Pro Mini. This project utilizes the ESP-NOW protocol to broadcast real-time I2S digital audio to 10 or more devices simultaneously without the need for a Wi-Fi router.

## ✨ Features

* **Multi-Node Broadcast:** Supports 10+ devices on a single network using ESP-NOW universal broadcast.
* **Application-Level Encryption:** Real-time XOR cipher to scramble audio data, ensuring privacy from unauthorized listeners.
* **Capacitive Touch PTT:** Utilizes a TTP223 touch sensor for instant Push-to-Talk functionality.
* **Power Management:** Double-tap the touch sensor to enter Deep Sleep; single touch to wake up.
* **Maximized Range:** Software-forced maximum TX power (19.5 dBm) optimized for external 6dBi U.FL antennas.
* **Core 3.x.x Compatible:** Written using the modern `ESP_I2S.h` object-oriented drivers for Arduino ESP32 Core v3.0+.

---

## 🛠️ Hardware Requirements

You will need the following components for **each** walkie-talkie node:

* **Microcontroller:** ESP32-C3 Pro Mini (with U.FL connector)
* **Antenna:** 2.4 GHz / 5 GHz External Antenna (6dBi) with U.FL/IPEX pigtail
* **Microphone:** INMP441 Omnidirectional I2S Digital Microphone
* **Amplifier:** MAX98357A I2S Class D Audio Amplifier
* **Speaker:** 4Ω or 8Ω speaker (1W - 3W)
* **PTT Sensor:** TTP223 Capacitive Touch Sensor
* **Power:** 3.7V LiPo Battery + TP4056 Module (or 5V USB-C power bank)

> **⚠️ Hardware Note for Antenna:** Ensure the 0-ohm resistor near the ESP32-C3's U.FL connector is bridged to the U.FL path, not the onboard PCB antenna.

---

## 🔌 Wiring Diagram

To maximize the ESP32-C3's limited GPIO pins, the Bit Clock (BCLK) and Word Select (LRC/WS) lines are shared between the microphone and the amplifier.

| Component | Pin | ESP32-C3 GPIO | Notes |
| :--- | :--- | :--- | :--- |
| **INMP441 (Mic)** | VDD | 3.3V | Power |
| | GND | GND | Ground |
| | L/R | GND | Sets mic to Left Channel |
| | SCK | GPIO 4 | I2S Bit Clock (Shared) |
| | WS | GPIO 5 | I2S Word Select (Shared) |
| | SD | GPIO 6 | I2S Data **IN** |
| **MAX98357A (Amp)**| VIN | 5V / VBUS | 5V provides louder audio |
| | GND | GND | Ground |
| | BCLK | GPIO 4 | I2S Bit Clock (Shared) |
| | LRC | GPIO 5 | I2S Word Select (Shared) |
| | DIN | GPIO 7 | I2S Data **OUT** |
| **TTP223 (Touch)** | VCC | 3.3V | Power |
| | GND | GND | Ground |
| | I/O | GPIO 8 | Push-to-Talk Signal (Active HIGH) |

*Wire the speaker directly to the + and - terminals on the MAX98357A board.*

---

## 💻 Software Setup

1. **Install Arduino IDE** and add the Espressif Board Manager URL.
2. Install the **esp32** board package (Must be version `3.0.0` or higher).
3. Select your board: `Tools > Board > ESP32C3 Dev Module`.
4. Open the provided sketch and locate the security settings:
   ```cpp
   // Change this to a unique string for your specific network
   const char* SECRET_PASSWORD = "YourCustomPasswordHere!";
