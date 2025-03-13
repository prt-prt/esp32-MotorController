# ESP32 Robot Car Controller

This project implements control software for a 2-wheel drive robot car using ESP32 microcontrollers with PWM speed control for the motors, enabling various movement modes.

## Project Structure

This project is organized into two versions:
- **v1**: Uses the ESP32-CAM and L298N motor driver (older, traditional driver)
- **v2**: Uses the ESP32-S2 Mini and DRV8833 motor driver (more efficient, modern setup)

Each version contains its own complete implementation for its respective hardware. For detailed wiring and implementation specifics, see the WIRING.md file in each subproject folder.

## Hardware Requirements

### Common Components for Both Versions
- 2-wheel drive robot car chassis with DC motors
- Jumper wires
- Small breadboard (optional, for cleaner wiring)

### Version Specific Components
- **v1**: 
  - ESP32-CAM development board
  - L298N dual H-bridge motor driver ([detailed documentation](v1/WIRING.md))
  - MP1584EN buck converter module (for power regulation)
  - Power supply for motors (battery pack/power bank)

- **v2**: 
  - Wemos ESP32-S2 Mini development board
  - DRV8833 dual H-bridge motor driver ([detailed documentation](v2/WIRING.md))
  - 5V powerbank with USB output (powers ESP32-S2 Mini directly)
  - Step-up converter (boosts 5V from powerbank to 6V for motors)
  - SPST toggle switch (for global power control)

### Power Supply Architecture Differences

The v1 and v2 versions use different power supply approaches:

**v1 Power Supply:**
- Battery/power source → MP1584EN buck converter → ESP32-CAM (5V)
- Battery/power source → L298N motor driver (higher voltage)

**v2 Power Supply:**
- Powerbank USB output → ESP32-S2 Mini USB-C port (direct connection)
- Powerbank 5V output → Toggle switch → Step-up converter → DRV8833 motor driver (6V)

The v2 setup includes a master power switch that allows you to disable the motors while keeping the microcontroller powered for programming and debugging.

## Hardware Comparison

### Microcontroller Comparison
| Feature | v1: ESP32-CAM | v2: ESP32-S2 Mini |
|---------|--------------|-------------------|
| USB Connection | External programmer required | Built-in USB-C port |
| Size | Larger | Compact |
| Power Input | 5V via pins | Direct USB-C |
| Programming | Via serial adapter | Native USB |
| Unnecessary Features | Camera module | None |
| Form Factor | Less robot-friendly | Ideal for robotics |

### Motor Driver Comparison
| Feature | v1: L298N | v2: DRV8833 |
|---------|----------|-------------|
| Efficiency | ~70% | >90% |
| Control Pins | 6 total | 4 total |
| Heat Generation | High | Low |
| Size | Large | Compact |
| Control Method | Enable + Direction pins | PWM on all pins |

For detailed pin connections, wiring diagrams, and implementation specifics, see:
- [L298N Wiring & Implementation Guide](v1/WIRING.md)
- [DRV8833 Wiring & Implementation Guide](v2/WIRING.md)

## Software Setup

This project uses PlatformIO for development and deployment:

1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Clone or download this repository
4. Open either the v1 or v2 project folder in VS Code
5. Build and upload the project using PlatformIO

### V2 Programming Advantage
The v2 version with ESP32-S2 Mini can be programmed directly via USB-C connection without additional hardware, making development and testing significantly easier.

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

You can customize the pin assignments at the beginning of the `motor_control.cpp` file if you need to use different GPIO pins on your microcontroller.

To implement additional movement modes or alter the existing ones, refer to the corresponding mode files in the `src` directory.

## Troubleshooting

Basic troubleshooting tips are included in the WIRING.md file for each version:
- [L298N Troubleshooting](v1/WIRING.md#troubleshooting)
- [DRV8833 Troubleshooting](v2/WIRING.md#troubleshooting)

## Project Structure

For each version (v1 and v2):
- `platformio.ini`: PlatformIO configuration
- `WIRING.md`: Detailed wiring and implementation documentation
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