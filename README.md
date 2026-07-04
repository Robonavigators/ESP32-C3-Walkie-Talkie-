# ESP32-C3 Walkie-Talkie Network

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status: Active](https://img.shields.io/badge/Status-Active-brightgreen.svg)
![Hardware: ESP32-C3](https://img.shields.io/badge/Hardware-ESP32--C3%20Pro%20Mini-blue.svg)
![Audio: I2S](https://img.shields.io/badge/Audio-I2S%20Digital-orange.svg)

A low-latency, encrypted, multi-device walkie-talkie system built on the ESP32-C3 Pro Mini. This project utilizes the ESP-NOW protocol to broadcast real-time I2S digital audio to 10 or more devices simultaneously without the need for a Wi-Fi router. 

It features an onboard web server with a modern Glassmorphism UI for configuring your network's encryption passcord on the fly.

## ✨ Features

* **Multi-Node Broadcast:** Supports 10+ devices on a single network using ESP-NOW universal broadcast.
* **Web-Based Configuration:** Built-in Access Point and web server (Glass UI) to save and update the encryption passcord to non-volatile memory (NVS) without reflashing code.
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

## ⚡ Installation

The easiest way to install the firmware is directly through your web browser using Web Serial:

1. Go to [Robonavigators ESP Flasher](https://robonavigators.github.io/flash.html).
2. Choose **ESP32-C3 Walkie Talkie Firmware** from the firmware menu.
3. Click the connect button and select your ESP32-C3's COM/Serial port from the browser popup.
4. Click upload to flash the firmware directly to your board.

## ⚙️ Network Configuration (Captive Portal)

On first boot, or if you need to change your network password, the ESP32-C3 will host its own Wi-Fi network to allow you to configure the shared Secret Passcord.

1. Power on the walkie-talkie. (If a passcord was already saved, **touch and hold the TTP223 sensor** while powering it on to force it into Setup Mode).
2. On your phone or computer, connect to the new Wi-Fi network:
   * **SSID:** `ESP32-C3 Encrypted Walkie talkie AP`
   * **Password:** `Password@123`
3. Open a web browser and navigate to `http://192.168.4.1`.
4. You will see a blue (`#0484f8`) screen with a Glassmorphism card. Enter your **Secret Passcord** (ensure this matches exactly across all your devices).
5. Click **Save & Restart**. The AP will turn off, and the device will reboot into normal Walkie-Talkie mode.

---

## 🚀 Usage Guide

* **Transmit:** Touch and hold your finger on the TTP223 sensor. Speak clearly into the INMP441 microphone.
* **Receive:** Remove your finger from the sensor. The device will automatically play incoming audio from any authorized node.
* **Sleep Mode:** Double-tap the TTP223 sensor quickly (within 1 second) to put the device into Deep Sleep and save battery.
* **Wake Up:** Touch the TTP223 sensor once to wake the device back up. It will be ready to transmit or receive in under 2 seconds.

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
