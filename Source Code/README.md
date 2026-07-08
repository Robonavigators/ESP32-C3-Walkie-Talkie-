# Source Code & Compilation Guide

This folder contains the raw Arduino C++ source code for the XIAO Walkie-Talkie project. 

If you prefer not to use the Web Flasher and want to compile the firmware yourself—or if you want to modify the code and build your own custom walkie-talkie network—follow the instructions below.

---

## ⚠️ Step 1: Secure Your Network (`secrets.h`)

Before you compile or upload anything, you **must** configure your network's unique stealth relay signature. 

By default, the `secrets.h` file contains placeholder values (`0xFF`). If you leave these as-is, your devices will be vulnerable to unauthorized relay pings from anyone else using the default code.

1. Open the `secrets.h` file in your editor.
2. Locate the `RELAY_HEADER` array:
   ```cpp
   const uint8_t RELAY_HEADER[] = {0xFF, 0xFF, 0xFF, 0xFF};
