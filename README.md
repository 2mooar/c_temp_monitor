# 🧭 Temperature Monitoring System

This project implements a modular **embedded temperature monitoring system** 

For more detailed information please read documents inside the **`docs`** folder


## 🚀 Build and Run in VS Code Dev Container

### 1. Open in VS Code
- Open this folder in **VS Code**.
- When prompted, select **“Reopen in Container”** to start the development environment.  
  (The container installs CMake, Ninja, GCC, and all required dependencies.)

### 2. Build using VS Code tasks
- Press **`Ctrl+Shift+B`** (or `Cmd+Shift+B` on macOS).  
- Select the task **Configure** then **Build** to compile the PC simulation version.

### 3. Run the executable
- Open the **VS Code terminal** inside the container.
- Run:
```bash
./build/app
```

Expected output:
```bash
*** Temperature Monitor System ***
*** System Ready ***
Serial Number: ABC1234
Hardware Revision: Rev-B (0.1°C/digit)
[LED] pin=10 -> OFF
[LED] pin=11 -> OFF
[LED] pin=12 -> OFF
[LED] pin=10 -> ON
[LED] pin=11 -> ON
[LED] pin=12 -> ON
[LED] pin=10 -> OFF
[LED] pin=11 -> OFF
[LED] pin=12 -> ON
[CRITICAL] 4.00 C
```
### 4. Stop the execution
Please press **`Ctrl+C`**

## 🧩 License
Internal prototype — for educational and evaluation purposes only.