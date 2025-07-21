# ESP8266 & LoRa MQTT Gateway

An IoT Gateway using an ESP8266 and a LoRa module to receive data from sensor nodes and forward it to an MQTT broker. This project is part of my portfolio for IoT and embedded systems.

## 🌟 Features

* **Wireless Communication:** Uses LoRa for long-range communication with sensor nodes.
* [cite_start]**WiFi & MQTT:** Connects to a WiFi network [cite: 1, 16] [cite_start]and sends data to an MQTT broker[cite: 2, 4, 10].
* [cite_start]**Dynamic Configuration:** Reads WiFi and LoRa settings from a `JSON` file stored in LittleFS[cite: 6, 8], allowing for easy setup without recompiling.
* [cite_start]**Status Pins:** Can read the status of digital and analog pins upon request[cite: 22, 23].

## 🛠️ Hardware

* ESP8266 Development Board (e.g., NodeMCU)
* LoRa Module (like SX1278)
* (Add any sensors you used)

## 📚 Libraries Used

* [cite_start]`ESP8266WiFi` [cite: 1]
* [cite_start]`PubSubClient` for MQTT [cite: 2]
* [cite_start]`ArduinoJson` for handling configuration [cite: 4]
* [cite_start]`LoRa` by Sandeep Mistry [cite: 5]
* [cite_start]`LittleFS` for on-chip file storage [cite: 6]

## ⚙️ Configuration

To configure the gateway, you need to upload a `JSON` file named `example.txt` to the ESP8266's LittleFS. [cite_start]The device listens on the Serial port for this JSON string upon startup[cite: 28].

The JSON structure should be as follows:
```json
{
  "ssid": "YOUR_WIFI_SSID",
  "password": "YOUR_WIFI_PASSWORD",
  "mqtt_server": "YOUR_MQTT_BROKER_IP",
  "mqtt_username": "YOUR_MQTT_USERNAME",
  "mqtt_password": "YOUR_MQTT_PASSWORD",
  "LB": 433E6,
  "SignalBandwidth": 125E3,
  "SpreadingFactor": 12,
  "SyncWord": 52,
  "CodingRate4": 5,
  "TxPower": 20,
  "Gain": 0
}
```

## 🚀 What I Learned

In this project, I practiced integrating multiple communication protocols (LoRa & WiFi). I learned how to manage device configurations dynamically using a file system on a microcontroller, which makes the device more flexible and user-friendly. [cite_start]It was also a great exercise in parsing JSON and handling different states of network connectivity[cite: 17, 30, 31, 32].

---
