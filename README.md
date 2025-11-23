# IoT Data Logging with ESP8266 — Local Dashboard with Bonus ThingSpeak Graph

## Overview
This project implements an **IoT data‑logging system** using an **ESP8266 (NodeMCU / Wemos D1 Mini)**. It captures temperature readings, displays them on a locally hosted **web dashboard**, and periodically uploads them to **ThingSpeak** for cloud‑based visualization. The system meets the criteria for **Task 2: IoT Data Logging (ESP8266 / NodeMCU)** of the *Arduino Winter Internship 2025*.

## Features
* ✔️ Reads temperature values from DHT11 sensor
* ✔️ Hosts a **modern, responsive HTML/CSS dashboard** on the ESP8266
* ✔️ Temperature status levels: COOL → NORMAL → WARM → HIGH
* ✔️ Auto‑refresh attempt using HTML META tag (implemented but **did not work**)
* ✔️ Periodic data upload to **ThingSpeak** (every 15 seconds)
* ✔️ Local dashboard updates reading every refresh
* ✔️ Live chart embedded from ThingSpeak on dashboard
* ✔️ Clean, glass‑morphism‑style UI with sensor cards

Screenshots and demo video are included in the submission repository.

## Hardware Requirements
* ESP8266 NodeMCU / Wemos D1 Mini
* DHT11 temperature sensor
* USB cable
* WiFi connection

## Software Requirements
* Arduino IDE
* ESP8266 Board Package
* Libraries:
  * `ESP8266WiFi.h`
  * `ESP8266HTTPClient.h`
  * `DHT.h`

## Project Logic Summary

### 1. **WiFi Setup**
The ESP8266 connects to the specified WiFi credentials. It waits until `WL_CONNECTED` before proceeding.

### 2. **Local Web Server**
A server runs on port **80**, serving an HTML dashboard with:
* current temperature reading
* temperature status indicators
* sensor cards (temperature active, others disabled)
* an embedded ThingSpeak graph

### 3. **Temperature Reading**
The function reads temperature from DHT11 sensor on pin D1:
```cpp
float temp = dht.readTemperature();
```

The value is classified into status levels:

| Range | Status |
|-------|--------|
| < 15°C | COOL |
| 15–28°C | NORMAL |
| 28–35°C | WARM |
| > 35°C | HIGH |

Temperature is updated every **5 seconds**.

### 4. **Dashboard Rendering (`serveDashboard()`)**
Generates full HTML with:
* gradients, glass‑morphism cards
* mobile‑friendly design
* real‑time value display

Includes:
```html
<meta http-equiv='refresh' content='5'>
```

Auto‑refresh tag — but **in testing, the auto-refresh did not work**.

### 5. **ThingSpeak Uploading**
Every **15 seconds**, the NodeMCU sends temperature data to the ThingSpeak API:
```
http://api.thingspeak.com/update?api_key=WRITE_KEY&field1=value
```

A live chart is embedded on the dashboard.

## Repository Contents
* `project.ino` — Arduino Sketch
* `README.md` — Documentation (this file)
* `/screenshots` — Dashboard + ThingSpeak screenshots
* `video.mp4` — Demo explaining setup

## How to Run

### 1. **Clone the repository**
```bash
git clone <repo-url>
cd esp8266-iot-dashboard
```

### 2. **Open in Arduino IDE**
Load the `.ino` file.

### 3. **Update WiFi Credentials**
```cpp
const char* ssid = "Your Wifi";
const char* password = "Your wifi password";
```

### 4. **Upload to ESP8266**
Select the correct board & COM port.

### 5. **Access the Dashboard**
Open a browser and visit:
```
http://<ESP8266-IP>
```

IP is shown in Serial Monitor.

## Task‑2 Requirements Checklist

| Requirement | Completed |
|--------------------------|-----------------------------------------------|
| ESP8266 used | ✔️ |
| At least 1 sensor | ✔️ (DHT11 Temperature Sensor) |
| Cloud OR Local dashboard | ✔️ Both implemented |
| Data upload every 5–10 sec | ✔️ (ThingSpeak every 15s, sensor read every 5s) |
| Graph/visualization | ✔️ ThingSpeak Chart + Embedded Dashboard View |
| Screenshots | ✔️ Provided |
| Video Demo | ✔️ Included |
| Well‑documented README | ✔️ This document |

## Notes
* Auto‑refresh was implemented using a meta tag but **did not function as expected** in the final dashboard.
* Flame and light sensor cards are placeholders (disabled) but styled to show UI scalability.

## License
MIT License — free to use and modify.

## Author
Sanjeev Krishna S

## Acknowledgements
This project was created as part of the **Arduino Winter Internship 2025 — Task 2 (IoT Data Logging)** for FOSSEE, IIT Bombay.