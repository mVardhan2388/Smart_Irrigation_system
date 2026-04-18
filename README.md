# 🌱 Smart Irrigation System using ESP32 & IoT

An IoT-based smart irrigation system that automates watering based on **soil moisture** and **weather forecast (rain probability)** using ESP32, sensors, and cloud integration.

---

## 🚀 Features

- 🌡 Real-time temperature & humidity monitoring (DHT22)
- 🌱 Soil moisture detection
- 🌧 Rain prediction using OpenWeather API
- ☁️ Cloud integration using ThingSpeak
- 🔄 Automatic irrigation control
- 💻 Live web dashboard for monitoring

---

## 🧠 System Overview

The system works in three main layers:

1. **Sensor Layer (ESP32)**
   - Reads temperature, humidity, and soil moisture

2. **Cloud Layer**
   - Weather data fetched using OpenWeather API
   - Data stored in ThingSpeak

3. **Application Layer**
   - Web dashboard displays real-time data
   - Decision logic controls irrigation

---

## ⚙️ Components Used

- ESP32 Microcontroller  
- DHT22 Temperature & Humidity Sensor  
- Capacitive Soil Moisture Sensor  
- Relay Module (Pump Control)  
- ThingSpeak Cloud Platform  
- OpenWeather API  

---

## 🔌 Circuit Diagram

![Circuit Diagram](circuit.png)

---

## 🧪 Experimental Setup

![Setup](setup.png)

---

## 📊 Dashboard Preview

![Dashboard](dashboard.png)

---

## 🔄 Working Principle

1. ESP32 reads:
   - Temperature
   - Humidity
   - Soil Moisture

2. Python script fetches:
   - Weather forecast
   - Rain probability

3. Data is uploaded to ThingSpeak

4. Decision Logic:

```cpp
if (soilValue > threshold && rainProb < 50) {
    Pump ON;
} else {
    Pump OFF;
}
