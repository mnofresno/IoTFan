# IoT Fan Controller

Welcome to the IoT Fan Controller project! This repository contains everything you need to build and control an IoT ceiling fan using an ESP32 microcontroller. The project is designed to be easy to set up and use, allowing you to control your fan remotely.

## Project Structure

This repository includes the following main components:

- **mqtt_transformer**: This module handles the MQTT protocol to communicate with the IoT device.
- **iot-fan-arduino**: This is the Arduino sketch that runs on the ESP32 microcontroller to control the fan.

## Getting Started

### Prerequisites

- An ESP32 microcontroller
- Basic knowledge of Arduino programming
- Node.js and npm installed on your machine

### Setup Instructions

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/iot-fan-controller.git
   cd iot-fan-controller
   ```

2. **Configure the Arduino Sketch**
   - Navigate to the `iot_fan_arduino` folder.
   - Rename the `.credentials.h.example` file to `.credentials.h` and update it with your MQTT credentials.

3. **Setting Up MQTT Transformer**
   - Navigate to the `mqtt_transformer` folder.
   - Rename the `credentials.json.example` file to `credentials.json` and enter your MQTT credentials.

4. **Install Dependencies**
   - For both `mqtt_transformer` and `frontend`, run:
   ```bash
   npm install
   ```

5. **Upload the Arduino Sketch**
   - Open the `iot_fan_arduino.ino` file in the Arduino IDE and upload it to your ESP32.

6. **Start the MQTT Transformer**
   ```bash
   cd mqtt_transformer
   npm start
   ```

7. **Run the Frontend**
   - Navigate to the `frontend` directory and start the server:
   ```bash
   npm start
   ```

8. **Access the Web Interface**
   - Open your web browser and go to `http://localhost:3000` to control your ceiling fan.

## Usage

- Use the web interface to turn the fan on or off.
- The current status of the fan will be displayed on the screen.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue if you have suggestions or improvements.

## License

This project is licensed under the ISC License. See the LICENSE file for more details.

## Acknowledgments

- Thanks to the open-source community for their contributions and support.
- Special thanks to the creators of the ESP32 and MQTT protocols.

Happy coding!
