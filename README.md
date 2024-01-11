# FireTrack - Embedded Programming

## FireTrack - Wildfire Management System: Embedded Components

FireTrack's embedded system plays a crucial role in the wildfire management and monitoring process. This README focuses on the embedded components of the FireTrack Wildfire Management System, specifically designed for real-time monitoring and efficient response to wildfire incidents.

### Overview

The embedded system of FireTrack consists of two main parts: `client_lora` and `server_wifi`. These components work together to gather environmental data from the field and transmit it to the central management system.

### Components

#### 1. `client_lora` - Data Collection Node
- **Hardware**: Adafruit Feather Bluefruit Sense equipped with the nRF52840 SoC.
- **LoRa Module**: Connected to Adafruit LoRa Radio FeatherWing - RFM95W.
- **Function**: It gathers sensor data and transmits it using the LoRa.
- **Programming Environment**: Developed using CircuitPython in Visual Studio.

#### 2. `server_wifi` - Data Relay Node
- **Hardware**: Adafruit HUZZAH32 – ESP32 Feather Board.
- **LoRa Module**: Integrated with Adafruit LoRa Radio FeatherWing - RFM95W.
- **Function**: It receives data from the `client_lora` nodes, connects to WiFi, and posts the data to the backend's endpoint.
- **Programming Environment**: Programmed using the Arduino framework.

### System Workflow

1. **Data Collection**: The `client_lora` (nRF52840) gathers environmental data and connects to the LoRa network.
2. **Data Transmission**: The collected data is sent to the `server_wifi` node.
3. **Data Relay**: The `server_wifi` (ESP32) receives the data, connects to a WiFi network, and forwards the data to the backend system.

The result can be seen:
- By using `/api/sensorData/{pointId}`: https://firetrack.cleverapps.io/swagger-ui/index.html#/
- On a website by picking point on a map and looking through its history: https://firetrack.cleverapps.io/

### Installation Guide

#### `client_lora` Setup

1. **Hardware Assembly**: Connect the Adafruit LoRa Radio FeatherWing - RFM95W to the Adafruit Feather Bluefruit Sense.
2. **Software Setup**:
    - Install Visual Studio.
    - Install the CircuitPython plugin in Visual Studio.
    - Clone the `client_lora` repository folder.
    - Load the CircuitPython code onto the Adafruit Feather Bluefruit Sense.

#### `server_wifi` Setup

1. **Hardware Assembly**: Attach the Adafruit LoRa Radio FeatherWing - RFM95W to the Adafruit HUZZAH32 – ESP32 Feather Board.
2. **Software Setup**:
    - Install the Arduino IDE.
    - Install the ESP32 board package in the Arduino IDE.
    - Clone the `server_wifi` repository folder.
    - Upload the Arduino sketch to the Adafruit HUZZAH32 board.

### Other Sub-Systems
- [**Frontend**](https://github.com/rabeezarre/firetrack-frontend): Interactive map displaying tracking points information.
- [**Mobile**](https://github.com/rabeezarre/firetrack-mobile): Tracking service for marking checked tracking points in real-time.
- [**Backend**:](https://github.com/rabeezarre/firetrack) Centralized database for web and mobile interfaces, built with Spring Boot using Java 17 and Gradle.

### Authors and Acknowledgment
- Author: rabeezarre [GitHub user](https://github.com/rabeezarre)
