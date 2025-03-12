# L298N Motor Driver Implementation Details

This document provides detailed information about the L298N motor driver implementation used in the v1 version of the ESP32 Robot Car Controller project.

## L298N Motor Driver Overview

The L298N is a traditional dual H-bridge motor driver that can control two DC motors independently. It has been widely used in robotics projects for many years.

### Key Characteristics
- Dual H-bridge motor driver
- Operating voltage: 5-35V
- Maximum current: 2A per channel
- Separate pins for direction control and speed control
- Lower efficiency (~70%)
- Higher heat generation (requires heatsink)
- Larger physical size

## Pin Configuration

The L298N motor driver requires 6 pins to control two motors:

### Control Pins
| ESP32 GPIO | L298N Pin | Function |
|------------|-----------|----------|
| GPIO2      | IN1       | Direction control for left motor |
| GPIO12     | IN2       | Direction control for left motor |
| GPIO14     | ENA       | PWM speed control for left motor |
| GPIO15     | IN3       | Direction control for right motor |
| GPIO13     | IN4       | Direction control for right motor |
| GPIO16     | ENB       | PWM speed control for right motor |

### Power Connections
| Power Source | L298N Pin | Function |
|--------------|-----------|----------|
| Battery +    | +12V      | Motor power input (6-12V recommended) |
| Battery -    | GND       | Ground connection |
| 5V           | +5V       | Logic power (jumper should be removed if using external 5V) |

## Detailed Wiring Diagram

```
Power Bank/Battery
│
├── + ──┬─── +12V (L298N Motor Power)
│       │
│       └─── VIN+ (MP1584EN Input)
│
└── - ──┬─── GND (L298N)
        │
        └─── VIN- (MP1584EN Input)

MP1584EN Buck Converter (Optional for ESP32 power)
│
├── VOUT+ ─── 5V (ESP32-CAM)
│
└── VOUT- ─── GND (ESP32-CAM)

ESP32-CAM                L298N
│                        │
├── GPIO2 ───────────── IN1
│                        │
├── GPIO12 ──────────── IN2
│                        │
├── GPIO14 ──────────── ENA
│                        │
├── GPIO15 ──────────── IN3
│                        │
├── GPIO13 ──────────── IN4
│                        │
├── GPIO16 ──────────── ENB
│                        │
└── GND ─────────────── GND
                         │
                         ├── OUT1 ─── Left Motor Terminal 1
                         │
                         ├── OUT2 ─── Left Motor Terminal 2
                         │
                         ├── OUT3 ─── Right Motor Terminal 1
                         │
                         └── OUT4 ─── Right Motor Terminal 2
```

## L298N Control Logic

The L298N driver uses a different control approach than modern drivers like the DRV8833:

1. **Direction Control**: The direction of each motor is controlled by setting the corresponding input pins (IN1/IN2 for left motor, IN3/IN4 for right motor)
2. **Speed Control**: The speed of each motor is controlled by applying a PWM signal to the enable pins (ENA for left motor, ENB for right motor)

### Direction Control Logic
| Motor Direction | IN1/IN3 | IN2/IN4 |
|-----------------|---------|---------|
| Forward         | HIGH    | LOW     |
| Backward        | LOW     | HIGH    |
| Stop            | LOW     | LOW     |
| Brake (lock)    | HIGH    | HIGH    |

### Robot Movement Control
| Robot Movement | Left Motor | Right Motor | IN1 | IN2 | IN3 | IN4 | ENA | ENB |
|----------------|------------|-------------|-----|-----|-----|-----|-----|-----|
| Forward        | Forward    | Forward     | HIGH| LOW | HIGH| LOW | PWM | PWM |
| Backward       | Backward   | Backward    | LOW | HIGH| LOW | HIGH| PWM | PWM |
| Turn Left      | Backward   | Forward     | LOW | HIGH| HIGH| LOW | PWM | PWM |
| Turn Right     | Forward    | Backward    | HIGH| LOW | LOW | HIGH| PWM | PWM |
| Stop           | Stop       | Stop        | LOW | LOW | LOW | LOW | LOW | LOW |

## Implementation Notes

### PWM Configuration
The implementation uses the ESP32's ledc functions for PWM generation:
- PWM Frequency: 5000 Hz
- PWM Resolution: 8-bit (0-255)
- PWM Channels: Channel 0 for ENA, Channel 1 for ENB

### Code Structure
The motor control implementation follows these key principles:
1. Direction pins (IN1, IN2, IN3, IN4) are configured as standard digital outputs
2. Enable pins (ENA, ENB) are configured as PWM outputs
3. High-level functions abstract the pin control details

### Key Functions

#### Motor Control
```cpp
// Set left motor speed and direction
void setMotorA(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  
  // Set speed via PWM on enable pin
  ledcWrite(PWM_CHANNEL_ENA, speed);
}

// Set right motor speed and direction
void setMotorB(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  // Set speed via PWM on enable pin
  ledcWrite(PWM_CHANNEL_ENB, speed);
}
```

### Differential Steering
The implementation supports differential steering, allowing for more organic and precise movements:
```cpp
// Function to move with different speeds for each wheel
// Positive values: forward, Negative values: backward
void moveDifferential(int leftSpeed, int rightSpeed) {
  // Determine direction and speed for left motor
  bool leftForward = (leftSpeed >= 0);
  int absLeftSpeed = abs(leftSpeed);
  
  // Determine direction and speed for right motor
  bool rightForward = (rightSpeed >= 0);
  int absRightSpeed = abs(rightSpeed);
  
  // Set motors
  setMotorA(absLeftSpeed, leftForward);
  setMotorB(absRightSpeed, rightForward);
}
```

## Limitations and Considerations

### Efficiency
The L298N has a voltage drop of approximately 1.5V, which means:
- Less efficient than modern drivers (~70% efficiency)
- Higher heat generation requiring a heatsink
- Reduced effective voltage to the motors (e.g., with a 7.4V battery, motors receive ~5.9V)

### Current Limitations
The L298N can provide up to 2A per channel, which is sufficient for most small to medium DC motors, but may not be enough for larger motors.

### Size Considerations
The L298N is larger than modern alternatives, which may be a consideration for space-constrained projects.

## Troubleshooting

### Motors Not Responding
- Check all control pin connections
- Verify power connections
- Ensure 5V logic power is available
- Confirm motor connections

### Motors Running In Wrong Direction
- Swap the motor terminal connections (OUT1/OUT2 or OUT3/OUT4)
- Or modify the control logic in the code to invert the direction

### Weak Motor Power
- Check battery voltage
- Ensure adequate power supply capacity
- Consider the voltage drop across the L298N
- Verify PWM signals on enable pins 