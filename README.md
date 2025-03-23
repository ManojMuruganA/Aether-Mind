# Real-Time Remote Patient Monitoring System

This repository contains the code for a **Real-Time Remote Patient Monitoring System** that collects and displays patient vitals such as **heart rate**, **SpO2**, **room temperature**, **room humidity**, **body temperature**, and **ECG/BPM**. The system uses IoT devices and sensors to provide real-time health monitoring for doctors and caregivers.

---

## Features
- **Real-Time Monitoring**: Collects and displays heart rate, SpO2, room temperature, room humidity, body temperature, and ECG/BPM.
- **Web Dashboard**: A web-based interface to visualize patient vitals in real-time.
- **Portable Design**: Uses affordable and accessible hardware components.

---

## Hardware Components
- **ESP8266**: For Wi-Fi connectivity and data transmission.
- **Arduino UNO R4 Minima**: For sensor data processing.
- **MAX30100**: Heart rate and SpO2 sensor.
- **DHT22**: Temperature and humidity sensor.
- **DS18B20**: Waterproof body temperature sensor.
- **BioAmp EXG Pill**: ECG sensor.

---

## Software Components
- **Arduino IDE**: For programming the ESP8266 and Arduino.

---

## Repository Structure
- **sketch_mar22a**: Collects and displays heart rate, SpO2, room temperature, room humidity, and body temperature.
- **sketch_mar23a**: Collects and displays ECG/BPM data.

---

## Setup Instructions

### Prerequisites
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the following libraries:
   - `MAX30100_PulseOximeter`
   - `DHT`
   - `OneWire`
   - `DallasTemperature`
   - `CircularBuffer`

### Hardware Setup
1. Connect the sensors to the ESP8266 and Arduino as per the circuit diagram.
2. Upload **sketch_mar22a** to the ESP8266 for heart rate, SpO2, temperature, and humidity monitoring.
3. Upload **sketch_mar23a** to the Arduino for ECG/BPM monitoring.

### Running the System
1. Power on the hardware.
2. Connect to the Wi-Fi network configured in the code.
3. Open the web dashboard by entering the ESP8266's IP address in a browser.
4. View real-time patient vitals on the dashboard.

---

## Contributors
- **Manoj**: Lead Developer, Hardware Integrator.
- **Chakka Sraeyes**: Backend Development (Arduino IDE).
- **Nooka Sai Krishna**: Backend and Frontend Development.
- **Eshwar Anand Rainikindi**: Frontend Development.
- **Sujith**: Testing and Documentation.

---

## Acknowledgments
- Thanks to the hackathon organizers for the opportunity.
- Special thanks to our mentors and teammates for their support.
