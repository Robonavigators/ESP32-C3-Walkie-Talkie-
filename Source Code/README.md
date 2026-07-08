# Source Code & Compilation Guide

This folder contains the raw Arduino C++ source code for the XIAO Walkie-Talkie project. 

If you want to modify the code, build your own custom walkie-talkie network, or simply prefer not to use the Web Flasher, follow these full instructions to compile and upload the firmware manually.

---

## ⚠️ Step 1: Secure Your Network (`secrets.h`)

Before you compile or upload anything, you **must** configure your network's unique stealth relay signature. 

By default, the `secrets.h` file contains placeholder values (`0xFF`). If you leave these as-is, your devices will be vulnerable to unauthorized relay pings from anyone else using the default code.

1. Open the `secrets.h` file in your editor.
2. Locate the `RELAY_HEADER` array:
   ```cpp
   const uint8_t RELAY_HEADER[] = {0xFF, 0xFF, 0xFF, 0xFF};
   ```
3. Change the four `0xFF` values to **your own random hexadecimal bytes** (any value from `0x00` to `0xFF`). 
   * *Example:* `{0x1A, 0x8B, 0x4C, 0xD9}`
4. **Crucial:** Make sure you flash the exact same `secrets.h` file to **all** walkie-talkies in your group. If the headers do not match perfectly, the stealth relay function will not work.

*(Note: Your actual voice encryption password is NOT set here. You will set that via the Web Captive Portal from your phone after turning the device on).*

---

## 🛠️ Step 2: IDE Environment Setup

This code relies on the modern ESP32 Core drivers. You must set up your Arduino IDE correctly to prevent compilation errors.

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Go to **File > Preferences** and add the following URL to the *Additional Boards Manager URLs* box:
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Go to **Tools > Board > Boards Manager**, search for `esp32` by Espressif Systems, and install it.
   * **⚠️ IMPORTANT:** You must install **Version 3.3.5** (or higher). Older 2.x.x versions will fail to compile because the ESP-NOW and I2S APIs have changed significantly.
4. Select your board: Go to **Tools > Board > esp32 > Seeed XIAO ESP32C3**.

---

## 📚 Step 3: Required Libraries

All the libraries used in this project are natively included within the ESP32 Core v3.3.5 environment. You **do not** need to download any external ZIP files from the Library Manager. 

The project utilizes the following built-in libraries:
* `<esp_now.h>` - Core radio protocol
* `<WiFi.h>` - AP Mode generation
* `<ESP_I2S.h>` - Core 3.x digital audio driver
* `<esp_sleep.h>` - Power management
* `<Preferences.h>` - Non-Volatile Storage (NVS)
* `<WebServer.h>` & `<DNSServer.h>` - Captive Portal UI

---

## 🚀 Step 4: Compile and Upload

1. Open the main `.ino` file in the Arduino IDE. (Ensure the `secrets.h` file is sitting in the exact same folder so it opens in an adjacent tab).
2. Connect your Seeed Studio XIAO ESP32-C3 to your computer via a data-capable USB-C cable.
3. Select the correct COM port under **Tools > Port**.
4. Click the **Upload** arrow at the top left of the IDE. 

Once the upload console says "Done," your XIAO will automatically reboot. If it is a fresh install, it will immediately host the `XIAO Walkie-Talkie` Wi-Fi network so you can connect with your phone and set your secret audio password!
