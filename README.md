# XIAO Walkie-Talkie

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status: Active](https://img.shields.io/badge/Status-Active-brightgreen.svg)
![Hardware: XIAO ESP32-C3](https://img.shields.io/badge/Hardware-XIAO%20ESP32--C3-blue.svg)
![Audio: I2S](https://img.shields.io/badge/Audio-I2S%20Digital-orange.svg)

A low-latency, encrypted, multi-device walkie-talkie system built on the Seeed Studio XIAO ESP32-C3. This project utilizes the ESP-NOW protocol to broadcast real-time I2S digital audio to 10 or more devices simultaneously without the need for a Wi-Fi router. 

It features an onboard Captive Portal with a modern Glassmorphism UI for configuring your network's encryption password on the fly, and smart thermal throttling to keep the tiny board cool during continuous use.

## ✨ Features

* **Multi-Node Broadcast:** Supports 10+ devices on a single network using ESP-NOW universal broadcast.
* **Captive Portal Configuration:** Built-in DNS server and Access Point (Glass UI) to easily save and update the encryption password to non-volatile memory (NVS).
* **Application-Level Encryption:** Real-time XOR cipher to scramble audio data, ensuring privacy from unauthorized listeners.
* **Smart Mechanical PTT:** Press and hold to transmit; single quick tap to reboot into network setup mode. Prevents accidental hot-mic broadcasts.
* **Thermal Throttling:** Automatically underclocks the CPU to 80MHz if the core temperature exceeds 65°C, restoring full 160MHz performance when cooled to 55°C.
* **Maximized Range:** Software-forced maximum TX power (19.5 dBm) optimized for the XIAO's U.FL external antenna.
* **Modern Core API:** Written using the `ESP_I2S.h` object-oriented drivers for Arduino ESP32 Core v3.3.5.

---

## 🛠️ Hardware Requirements

You will need the following components for **each** walkie-talkie node:

* **Microcontroller:** Seeed Studio XIAO ESP32-C3 (with U.FL antenna included)
* **Microphone:** INMP441 Omnidirectional I2S Digital Microphone
* **Amplifier:** MAX98357A I2S Class D Audio Amplifier
* **Speaker:** 4Ω or 8Ω speaker (1W - 3W)
* **PTT Sensor:** Standard momentary push button
* **Power:** 3.7V LiPo Battery (wired to battery pads) or USB-C.

---

## 🔌 Wiring Diagram

To maximize the XIAO's limited GPIO pins, the Bit Clock (BCLK) and Word Select (LRC/WS) lines are shared between the microphone and the amplifier.

| Component | Pin | XIAO Pin (GPIO) | Notes |
| :--- | :--- | :--- | :--- |
| **INMP441 (Mic)** | VDD | 3.3V | Power |
| | GND | GND | Ground |
| | L/R | GND | Sets mic to Left Channel |
| | SCK | D2 (GPIO 4) | I2S Bit Clock (Shared) |
| | WS | D3 (GPIO 5) | I2S Word Select (Shared) |
| | SD | D4 (GPIO 6) | I2S Data **IN** |
| **MAX98357A (Amp)**| VIN | 5V | 5V provides louder audio |
| | GND | GND | Ground |
| | BCLK | D2 (GPIO 4) | I2S Bit Clock (Shared) |
| | LRC | D3 (GPIO 5) | I2S Word Select (Shared) |
| | DIN | D5 (GPIO 7) | I2S Data **OUT** |
| **Push Button** | Leg 1 | D8 (GPIO 8) | Uses internal pull-up |
| | Leg 2 | GND | Connects to ground when pressed |

*Wire the speaker directly to the + and - terminals on the MAX98357A board.*

---

## ⚡ Installation

The easiest way to install the firmware is directly through your web browser using Web Serial:

1. Go to [Robonavigators ESP Flasher](https://robonavigators.github.io/flash.html).
2. Choose **XIAO Walkie-Talkie Firmware** from the firmware menu.
3. Click the connect button and select your XIAO ESP32-C3's COM/Serial port from the browser popup.
4. Click upload to flash the firmware directly to your board.

---

## ⚙️ Network Configuration (Captive Portal)

On first boot, or if you need to change your network password, the XIAO will host its own Wi-Fi network and pop up a configuration portal.

1. Power on the walkie-talkie. (If a password was already saved and you want to change it, **give the PTT button a single quick tap** to force it into Setup Mode).
2. On your phone or computer, connect to the new Wi-Fi network:
   * **SSID:** `XIAO Walkie-Talkie`
   * **Password:** `Password@123`
3. A login portal should automatically appear on your screen. (If it does not, open a web browser and navigate to `http://192.168.4.1`).
4. You will see a blue (`#0484f8`) screen with a Glassmorphism card. Enter your **Secret Password** (ensure this matches exactly across all your devices).
5. Click **Save & Restart**. The AP will turn off, and the device will reboot into normal Walkie-Talkie mode.

---

## 🚀 Usage Guide

* **Transmit:** Press and hold the PTT button for more than 400 milliseconds. Speak clearly into the INMP441 microphone.
* **Receive:** Release the button. The device will automatically play incoming audio from any authorized node.
* **Setup Mode:** Tap the button quickly (less than 400ms) to trigger a reboot directly into the Wi-Fi Captive Portal for password changes.

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
