# ESP32 Robot Car Controller

This project implements control software for a 2-wheel drive robot car using an ESP32 microcontroller with PWM speed control for the motors, enabling various movement modes.

## Hardware Requirements

- ESP32-CAM development board
- 2-wheel drive robot car chassis with DC motors
- DRV8833 dual H-bridge motor driver
- MP1584EN buck converter module (for power regulation)
- Power supply for motors (battery pack/power bank)
- Jumper wires
- Small breadboard (optional, for cleaner wiring)

## Pin Connections

The project uses the following pin connections between the ESP32-CAM and the DRV8833 motor driver:

### Motor Control Pins
- **AIN1**: GPIO2 - PWM control for left motor direction 1
- **AIN2**: GPIO12 - PWM control for left motor direction 2
- **BIN1**: GPIO15 - PWM control for right motor direction 1
- **BIN2**: GPIO14 - PWM control for right motor direction 2

## Comprehensive Wiring Guide

### Power Circuit

1. **Battery/Power Bank** → **MP1584EN Buck Converter**
   - Connect the positive terminal of your power source to VIN+ on the MP1584EN
   - Connect the negative terminal of your power source to VIN- on the MP1584EN

2. **MP1584EN Buck Converter** → **ESP32-CAM**
   - Set the MP1584EN output to 5V using its adjustment potentiometer
   - Connect VOUT+ from the MP1584EN to 5V on the ESP32-CAM
   - Connect VOUT- from the MP1584EN to GND on the ESP32-CAM

3. **Battery/Power Bank** → **DRV8833 VCC**
   - Connect the positive terminal directly to VM (motor power) on the DRV8833
   - Connect the negative terminal to GND on the DRV8833

4. **Common Ground**
   - Connect GND from the MP1584EN, DRV8833, and ESP32-CAM together

### Motor Connections

1. **DRV8833** → **Motors**
   - Connect Motor A (Left):
     - AOUT1 → Left motor terminal 1
     - AOUT2 → Left motor terminal 2
   - Connect Motor B (Right):
     - BOUT1 → Right motor terminal 1
     - BOUT2 → Right motor terminal 2

2. **ESP32-CAM** → **DRV8833 Control Pins**
   - GPIO2 → AIN1
   - GPIO12 → AIN2
   - GPIO15 → BIN1
   - GPIO14 → BIN2

### Wiring Diagram

```
Power Bank/Battery
│
├── + ──┬─── VM (DRV8833 Motor Power)
│       │
│       └─── VIN+ (MP1584EN Input)
│
└── - ──┬─── GND (DRV8833)
        │
        └─── VIN- (MP1584EN Input)

MP1584EN Buck Converter
│
├── VOUT+ ─── 5V (ESP32-CAM)
│
└── VOUT- ─── GND (ESP32-CAM)

ESP32-CAM                DRV8833
│                        │
├── GPIO2 ───────────── AIN1
│                        │
├── GPIO12 ──────────── AIN2
│                        │
├── GPIO15 ──────────── BIN1
│                        │
├── GPIO14 ──────────── BIN2
│                        │
└── GND ─────────────── GND
                         │
                         ├── AOUT1 ─── Left Motor Terminal 1
                         │
                         ├── AOUT2 ─── Left Motor Terminal 2
                         │
                         ├── BOUT1 ─── Right Motor Terminal 1
                         │
                         └── BOUT2 ─── Right Motor Terminal 2
```

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

## DRV8833 Motor Control

The DRV8833 motor driver operates differently from traditional drivers:

- Uses 4 PWM control pins instead of separate direction and enable pins
- Controls both direction and speed with the same pins
- For each motor:
  - Forward: PWM on AIN1/BIN1, 0V on AIN2/BIN2
  - Reverse: 0V on AIN1/BIN1, PWM on AIN2/BIN2  
  - Coast (free rotation): 0V on both inputs
  - Brake: High signal on both inputs

Benefits of using the DRV8833:
- Higher efficiency (>90% vs ~70% with L298N)
- Lower heat generation
- Better battery life
- Smaller size

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

### Power Issues
- If the ESP32 doesn't power on, check that the MP1584EN is properly set to 5V output
- Verify all ground connections are properly connected
- Ensure your power source has sufficient capacity for both the ESP32 and motors

### Motor Issues
- If motors don't respond, check all control pin connections
- If motors run in the wrong direction, swap the motor's terminal connections
- Low motor power could indicate insufficient battery voltage

### Debugging
- Serial monitoring (115200 baud rate) will provide debugging information
- The ESP32-CAM's onboard LED can be used for status indications

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