# ESP32 Robot Car Controller

This project implements control software for a 2-wheel drive robot car using an ESP32 microcontroller with PWM speed control for the motors, enabling various movement modes.

## Hardware Requirements

- ESP32 development board
- 2-wheel drive robot car chassis with DC motors
- L298N Motor Driver or similar
- Power supply for motors (battery pack)
- Jumper wires

## Pin Connections

The project uses the following pin connections between the ESP32 and the L298N motor driver:

### Motor A (Left Motor)
- ENA: GPIO2 (PWM speed control)
- IN1: GPIO12 (Direction control)
- IN2: GPIO13 (Direction control)

### Motor B (Right Motor)
- ENB: GPIO15 (PWM speed control)
- IN3: GPIO14 (Direction control)
- IN4: GPIO0 (Direction control)

## Software Setup

This project uses PlatformIO for development and deployment:

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Clone or download this repository
4. Open the project folder in VS Code
5. Build and upload the project using PlatformIO

## Features

### Movement Modes

The robot car supports three different movement modes:

1. **Test Mode**: Demonstrates basic movements in a sequence
2. **Wander Mode**: Robot randomly wanders with organic movements
3. **Rotate Mode**: Robot slowly rotates around its axis

By default, the robot switches between Wander Mode and Rotate Mode every 30 seconds.

### Basic Movement Functions

The code includes the following standard movement functions:

- `moveForward(speed)`: Moves the robot forward
- `moveBackward(speed)`: Moves the robot backward
- `turnLeft(speed)`: Turns the robot left
- `turnRight(speed)`: Turns the robot right
- `stopMotors()`: Stops all motor movement

### Differential Steering

Advanced movement control is achieved through differential steering:

- `moveDifferential(leftSpeed, rightSpeed)`: Allows independent control of each wheel
- `curveLeft(leftSpeed, rightSpeed)`: Creates curved left movement
- `curveRight(leftSpeed, rightSpeed)`: Creates curved right movement

### Speed Control

All movement functions support variable speed control through PWM, with speed values ranging from 0 (stop) to 255 (full speed).

## Movement Modes in Detail

### Test Mode

Cycles through a series of basic movements to test motor functionality:
1. Forward → Stop → Backward → Stop → Left turn → Stop → Right turn → Stop

### Wander Mode

Implements organic, randomized movement with varying speeds and paths:
- Straight forward movement
- Gentle left and right curves
- Sharp left and right curves
- 180° spin turns
- Backward movement
- Random stops

### Rotate Mode

Performs a slow, controlled rotation around the robot's center:
- Rotates in a random direction (left or right)
- Changes direction every 10 seconds
- Uses reduced speed for careful rotation

## Customization

You can customize the pin assignments at the beginning of the `motor_control.cpp` file if you need to use different GPIO pins on your ESP32.

To implement additional movement modes or alter the existing ones, refer to the corresponding mode files in the `src` directory.

## Troubleshooting

- If the robot doesn't move, check all wiring connections.
- If motors are spinning in the wrong direction, swap the IN1/IN2 or IN3/IN4 connections.
- Ensure your motor power supply has sufficient voltage and current.
- Serial monitoring (115200 baud rate) will provide debugging information.

## Project Structure

- `platformio.ini`: PlatformIO configuration
- `include/motor_control.h`: Header file for motor control functions
- `include/movement_modes.h`: Defines the movement modes enum
- `include/test_mode.h`: Header for test mode
- `include/wander_mode.h`: Header for wander mode
- `include/rotate_mode.h`: Header for rotate mode
- `src/motor_control.cpp`: Motor control functions implementation
- `src/main.cpp`: Main program with setup, loop, and mode switching logic
- `src/test_mode.cpp`: Test mode implementation
- `src/wander_mode.cpp`: Wander mode implementation with organic movement
- `src/rotate_mode.cpp`: Rotate mode implementation

## License

This project is open source and available under the MIT License. 