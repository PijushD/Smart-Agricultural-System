# Smart Agriculture with AI Monitoring System

An IoT‑based smart agriculture system that monitors key environmental parameters and controls farm actuators wirelessly using an ESP32 microcontroller and a web dashboard.

## Features

- Real‑time monitoring of:
  - Soil moisture
  - Temperature and humidity
  - Water level (excess water detection)[1]
- Wireless control of:
  - Irrigation pump
  - Fertilizer pump
  - Pesticide pump
  - Excess water removal pump
  - Artificial grow lights[1]
- Built‑in Wi‑Fi Access Point (`SMART_AGRICULTURAL_CONTROL`) for direct phone‑to‑device connection (no internet required).[1]
- Web interface with:
  - Live sensor values (auto‑refresh)
  - ON/OFF control buttons
  - PWM sliders for speed/brightness control[1]
- Modular design suitable for small and medium farms.

## Hardware Overview

- **ESP32 development board** (main controller, Wi‑Fi AP + web server)[1]
- **DHT11 sensor** for temperature and humidity  
- **Capacitive soil moisture sensor**  
- DC pumps for water, fertilizer, pesticide, and drainage  
- LED grow lights  
- Relay module / MOSFET drivers  
- 5V/12V power supply (or battery + solar, optional)[1]

## Software Overview

- Firmware written in **Arduino (C/C++)** using:
  - `WiFi.h` / `ESPAsyncWebServer` or `WebServer` (HTTP server)
  - DHT sensor library  
- Web dashboard built with **HTML5, CSS3, JavaScript** (AJAX `fetch()` for live updates).
- No external database required; all logic runs on the ESP32.

## How It Works

1. Sensors periodically measure soil moisture, temperature, humidity, and other parameters.  
2. ESP32 processes these readings and exposes them as JSON over `/getSensorData`.  
3. A built‑in web page (hosted on the ESP32) shows live data and control buttons/sliders.  
4. User actions call endpoints like `/toggleLED` and `/setBrightness`, which switch pumps/lights and adjust PWM duty cycles.  
5. Optional automation rules (e.g., soil moisture below threshold) can turn irrigation on/off automatically.

## Getting Started

### Prerequisites

- Arduino IDE with ESP32 board support installed  
- Required libraries:
  - `WiFi` / `WebServer` (ESP32 core)
  - DHT sensor library (Adafruit or equivalent)  

### Setup

1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/smart-agriculture-ai-monitoring.git
   cd smart-agriculture-ai-monitoring
   ```
2. Open the main `.ino` file in Arduino IDE.  
3. Select the correct **Board** (e.g., ESP32 Dev Module) and **Port**.  
4. Upload the sketch to the ESP32.  
5. After reset, connect your phone/laptop to the Wi‑Fi network `SMART_AGRICULTURAL_CONTROL`.  
6. Open a browser and go to `http://192.168.4.1` to access the dashboard.

## Repository Structure (suggested)

- `firmware/` – ESP32 Arduino code  
- `web/` – HTML/CSS/JS files (if served separately)  
- `docs/` – Project report, circuit diagrams, and screenshots  
- `images/` – UI and hardware photos  

## Team

- **Debagnik Roy** – Team Leader (Hardware & Software)  
- **Sraya Dutta Chowdhury** – Member (Software)  
- **Pijush Das** – Member (Software)
