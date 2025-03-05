# ESP32 Robot Car Controller

This project implements control software for a 2-wheel drive robot car using an ESP32 microcontroller and direct GPIO control for the motors.

## Hardware Requirements

- ESP32 development board
- 2-wheel drive robot car chassis with DC motors
- Power supply for motors (battery pack)
- Jumper wires

## Pin Connections

The project uses the following pin connections between the ESP32 and the motors:

### Motor A (Left Motor)
- IN1: GPIO12 (Direction control)
- IN2: GPIO13 (Direction control)

### Motor B (Right Motor)
- IN3: GPIO14 (Direction control)
- IN4: GPIO15 (Direction control)

## Software Setup

This project uses PlatformIO for development and deployment:

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Clone or download this repository
4. Open the project folder in VS Code
5. Build and upload the project using PlatformIO

## Features

### Basic Movement Functions

The code includes the following movement functions:

- `moveForward(speed)`: Moves the robot forward
- `moveBackward(speed)`: Moves the robot backward
- `turnLeft(speed)`: Turns the robot left
- `turnRight(speed)`: Turns the robot right
- `stopMotors()`: Stops all motor movement

### Demo Mode

The default program demonstrates these movements in a sequence:
1. Move forward for 2 seconds
2. Stop for 1 second
3. Move backward for 2 seconds
4. Stop for 1 second
5. Turn left for 2 seconds
6. Stop for 1 second
7. Turn right for 2 seconds
8. Stop for 1 second
9. Repeat

## Customization

You can customize the pin assignments at the beginning of the `motor_control.cpp` file if you need to use different GPIO pins on your ESP32.

To implement your own control logic, modify the `loop()` function in `main.cpp`.

## Troubleshooting

- If the robot doesn't move, check all wiring connections.
- If motors are spinning in the wrong direction, swap the IN1/IN2 or IN3/IN4 connections.
- Make sure your motor power supply has sufficient voltage and current.
- Serial monitoring (115200 baud rate) will provide debugging information.

## Project Structure

- `platformio.ini`: PlatformIO configuration
- `src/motor_control.cpp`: Motor control functions implementation
- `include/motor_control.h`: Header file for motor control functions
- `src/main.cpp`: Main program with demo sequence

## License

This project is open source and available under the MIT License. 