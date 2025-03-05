# ESP32 Robot Car Controller

This project implements control software for a 2-wheel drive robot car using an ESP32-CAM microcontroller and an L298N Motor Driver.

## Hardware Requirements

- ESP32-CAM development board
- L298N Motor Driver
- 2-wheel drive robot car chassis with DC motors
- Power supply for motors (battery pack)
- Jumper wires
- Optional: Breadboard for connections
- **FTDI USB-to-TTL Serial Adapter (for programming the ESP32-CAM)**

## ESP32-CAM Programming Setup

The ESP32-CAM does not have a built-in USB interface for programming. You must use an FTDI adapter or similar USB-to-TTL converter with the following connections:

```
FTDI Adapter     ESP32-CAM
-----------      ---------
5V/VCC      ---> 5V
GND         ---> GND
TX          ---> RX (GPIO3)
RX          ---> TX (GPIO1)
```

**Important: You must connect GPIO0 to GND during the upload process to enable bootloader mode, then disconnect after upload is complete.**

Proper upload procedure:
1. Connect the ESP32-CAM to the FTDI adapter as shown above
2. Connect GPIO0 to GND
3. Press the RESET button on ESP32-CAM
4. Start the upload in PlatformIO
5. When upload completes, disconnect GPIO0 from GND
6. Press RESET again to start running your code

![ESP32-CAM Programming Setup](https://i.imgur.com/oMgzLz6.png)

## Pin Connections

The project uses the following pin connections between the ESP32-CAM and L298N Motor Driver:

### Motor A (Left Motor)
- ENA: GPIO2 (PWM pin for controlling motor speed)
- IN1: GPIO14 (Direction control)
- IN2: GPIO15 (Direction control)

### Motor B (Right Motor)
- ENB: GPIO13 (PWM pin for controlling motor speed)
- IN3: GPIO12 (Direction control)
- IN4: GPIO4 (Direction control)

## Wiring Diagram

```
ESP32-CAM            L298N
---------            ---------
GPIO2  -------->  ENA
GPIO14 -------->  IN1
GPIO15 -------->  IN2
GPIO13 -------->  ENB
GPIO12 -------->  IN3
GPIO4  -------->  IN4
GND    -------->  GND
```

## Software Setup

This project uses PlatformIO for development and deployment. Follow these steps to get started:

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Clone or download this repository
4. Open the project folder in VS Code
5. Build the project using the PlatformIO build button
6. Connect your ESP32-CAM to your computer via the FTDI adapter (follow the ESP32-CAM Programming Setup section)
7. Upload the code using the PlatformIO upload button

## Features

### Basic Movement Functions

The code includes the following movement functions:

- `moveForward(speed)`: Moves the robot forward at the specified speed
- `moveBackward(speed)`: Moves the robot backward at the specified speed
- `turnLeft(speed)`: Turns the robot left at the specified speed
- `turnRight(speed)`: Turns the robot right at the specified speed
- `stopMotors()`: Stops all motor movement

### Demo Mode

The default program demonstrates these movements in a sequence:
1. Move forward for 2 seconds
2. Stop for 1 second
3. Move backward for 2 seconds
4. Stop for 1 second
5. Turn left for 1 second
6. Stop for 1 second
7. Turn right for 1 second
8. Stop for 1 second
9. Repeat

## Customization

You can customize the pin assignments at the beginning of the `motor_control.cpp` file if you need to use different GPIO pins on your ESP32-CAM.

To implement your own control logic, modify the `loop()` function in `main.cpp`.

## ESP32-CAM Notes

The ESP32-CAM has limited GPIO pins available as many are used for the camera functionality:
- GPIO0 is connected to the on-board LED and also affects boot mode
- GPIO1 and GPIO3 are used for UART (serial communication)
- GPIO2 can be used but is also connected to an on-board LED
- GPIO16 and GPIO17 are typically used for PSRAM
- Some pins are not exposed or are connected to the SD card and camera
- Use the pins specified in this project as they should be available for general I/O

## Troubleshooting

- If the robot doesn't move, check all wiring connections.
- If motors are spinning in the wrong direction, swap the IN1/IN2 or IN3/IN4 connections.
- Make sure your motor power supply has sufficient voltage and current.
- Serial monitoring (115200 baud rate) will provide debugging information.

### Upload Issues
- If you get "Unable to verify flash chip connection" errors, make sure:
  1. GPIO0 is properly connected to GND during upload
  2. You pressed the RESET button just before uploading
  3. You have proper connections between FTDI and ESP32-CAM
  4. Try a lower upload_speed in platformio.ini (we've set it to 115200)
  5. Try a different USB cable or USB port
  6. Some FTDI adapters may need the DTR and RTS pins to be disabled or controlled differently

## Project Structure

- `platformio.ini`: PlatformIO configuration
- `src/motor_control.cpp`: Motor control functions implementation
- `include/motor_control.h`: Header file for motor control functions
- `src/main.cpp`: Main program with demo sequence

## License

This project is open source and available under the MIT License. 