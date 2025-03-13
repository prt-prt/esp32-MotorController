# DRV8833 Motor Driver Implementation with ESP32-S2 Mini

This document provides detailed information about the DRV8833 motor driver implementation used in the v2 version of the ESP32 Robot Car Controller project, using a Wemos ESP32-S2 Mini as the microcontroller.

## Hardware Components

- **Wemos ESP32-S2 Mini** microcontroller
- **DRV8833** dual H-bridge motor driver
- **Step-up converter** (5V to 6V)
- **SPST toggle switch** (for global power control)
- **Powerbank** with USB output
- **USB splitter cable or adapter** (to split USB power after the switch)
- **DC motors** (2x)
- **Jumper wires and connectors**

## ESP32-S2 Mini Overview

The Wemos ESP32-S2 Mini offers significant advantages for this robot project:

- Built-in USB-C port for programming and power
- Small form factor (ideal for compact robots)
- Native USB support
- Multiple GPIO pins available for motor control
- 3.3V logic level
- Built-in power regulation
- Low power consumption

## DRV8833 Motor Driver Overview

The DRV8833 is a modern dual H-bridge motor driver that can control two DC motors independently. It offers significant advantages over older drivers like the L298N in terms of efficiency, size, and power consumption.

### Key Characteristics
- Dual H-bridge motor driver
- Operating voltage: 2.7V to 10.8V
- Maximum continuous current: 1.5A per channel (2A peak)
- PWM speed and direction control through the same pins
- High efficiency (>90%)
- Low heat generation
- Small physical footprint
- Built-in protection features (overcurrent, overtemperature, undervoltage)

## Pin Configuration

### Control Pins
| ESP32-S2 Mini GPIO | DRV8833 Pin | Function |
|--------------------|-------------|----------|
| GPIO7              | AIN1        | PWM control for left motor direction 1 |
| GPIO8              | AIN2        | PWM control for left motor direction 2 |
| GPIO9              | BIN1        | PWM control for right motor direction 1 |
| GPIO10             | BIN2        | PWM control for right motor direction 2 |

*Note: The exact GPIO numbers should be adjusted based on your specific board layout and requirements.*

### Power Connections
| Power Source | Connected To | Function |
|--------------|-------------|----------|
| Powerbank USB output | Global toggle switch | Main power input for entire system |
| Toggle switch output | USB splitter | Distributes power to both systems |
| USB splitter - first output | ESP32-S2 Mini USB-C | Power for microcontroller |
| USB splitter - second output | 5V to Step-up converter | Input for motor power circuit |
| Step-up converter output (6V) | DRV8833 VM | Motor power supply (6V recommended) |
| GND connections | Common ground | All grounds must be connected together |

## Power Supply Architecture

This implementation uses a true global power control approach:

1. **Powerbank** serves as the single power source:
   - USB output goes to the global toggle switch

2. **Global Power Switch**:
   - SPST toggle switch placed between the powerbank and all power consumers
   - Provides master on/off control for the entire system
   - When off, both the ESP32-S2 Mini and motors are powered down
   - When on, power flows to both the ESP32-S2 Mini and the step-up converter

3. **USB Power Distribution**:
   - After the switch, a USB splitter/adapter divides power to:
     1. ESP32-S2 Mini via USB-C connector (for both power and programming)
     2. Step-up converter input (via USB power extraction)

4. **Step-up Converter**:
   - Boosts the 5V from the USB power to 6V
   - Powers the motors through the DRV8833 driver (VM pin)

This architecture provides simplified control with a single switch while maintaining clean, separate power paths for the sensitive microcontroller and the more demanding motors.

## Detailed Wiring Diagram

```
Powerbank
│
└── USB output ──── Global Toggle Switch ──── USB Splitter/Hub
                                               │
                                               ├─── USB-C to ESP32-S2 Mini
                                               │
                                               └─── USB power extraction
                                                    │
                                                    │ (5V + GND)
                                                    │
                                                    └─── Step-up Converter
                                                         │
                                                         │ (6V output)
                                                         │
                                                         └─── VM (DRV8833)

ESP32-S2 Mini            DRV8833
│                        │
├── GPIO7 ────────────── AIN1
│                        │
├── GPIO8 ────────────── AIN2
│                        │
├── GPIO9 ────────────── BIN1
│                        │
├── GPIO10 ───────────── BIN2
│                        │
└── GND ─────────────────┼─── GND
                         │
                         ├─── AOUT1 ─── Left Motor Terminal 1
                         │
                         ├─── AOUT2 ─── Left Motor Terminal 2
                         │
                         ├─── BOUT1 ─── Right Motor Terminal 1
                         │
                         └─── BOUT2 ─── Right Motor Terminal 2

All GND connections must be common between:
- ESP32-S2 Mini
- DRV8833
- Step-up converter
- Powerbank negative output
```

## Implementing the Global Power Switch

There are two recommended approaches for implementing the global power switch:

### Option 1: USB Power Switch (Preferred)
- Use a dedicated USB power switch that handles all 4 USB lines
- This allows for data connectivity when needed
- Examples: USB in-line switch or USB power switch module

### Option 2: DIY Approach
- Use a SPST toggle switch placed in the USB power line (VBUS/red wire only)
- Cut only the red wire in a USB cable and connect the toggle switch
- Ensure the switch is rated for at least 2-3A
- Leave the data lines (D+/D-) and ground wire intact

## Power Supply Considerations

### Powerbank Selection
- Choose a powerbank with at least 2A output capability 
- Standard USB output will be split between ESP32-S2 Mini and the motor circuit
- For longer run time, select a powerbank with higher capacity (10,000mAh+)
- Some powerbanks may turn off automatically with low current draw, so test before final implementation

### Global Power Switch
- SPST (Single Pole, Single Throw) toggle switch for DIY approach
- USB power switch for cleaner implementation 
- Should be rated for at least 2-3A
- Position the switch in an easily accessible location on the robot
- Provides convenient on/off control for the entire system

### USB Splitter/Power Distribution
- Use a high-quality USB splitter or hub that can handle 2A+ total current
- For DIY approach, you can extract 5V/GND from a USB breakout board or adapter
- Ensure all power connections use adequately sized wires

### Step-up Converter for Motors
- Converts 5V to 6V for optimal motor performance
- The DRV8833 works well with 6V for small DC motors
- Higher voltage provides better torque and speed
- Efficiency is typically around 85-90%
- Must be able to handle the peak current of both motors running simultaneously

### ESP32-S2 Mini Power
- Powered directly via USB-C port from the USB splitter
- Has built-in power regulation
- The power switch controls both ESP32-S2 Mini and motors for true global control

### Common Ground Connection
- All ground connections must be tied together
- This ensures proper signal transmission and prevents floating reference issues
- Create a central ground point or "star ground" for all components

## DRV8833 Control Logic

The DRV8833 uses a more streamlined approach to motor control compared to traditional drivers:

1. **Both Direction and Speed Control**: Each motor requires two PWM pins (AIN1/AIN2 for left motor, BIN1/BIN2 for right motor)
2. **No Separate Enable Pins**: Direction and speed are controlled by the PWM duty cycle on the input pins

### Motor Control Logic
| Motor Function | AIN1/BIN1 | AIN2/BIN2 | Result |
|----------------|-----------|-----------|--------|
| Forward        | PWM       | 0         | Motor rotates forward at speed proportional to PWM |
| Backward       | 0         | PWM       | Motor rotates backward at speed proportional to PWM |
| Coast (free)   | 0         | 0         | Motor is disconnected (free rotation) |
| Brake (lock)   | 1         | 1         | Motor terminals are shorted (active braking) |

### Robot Movement Control
| Robot Movement | Left Motor      | Right Motor     |
|----------------|-----------------|-----------------|
| Forward        | AIN1=PWM, AIN2=0 | BIN1=PWM, BIN2=0 |
| Backward       | AIN1=0, AIN2=PWM | BIN1=0, BIN2=PWM |
| Turn Left      | AIN1=0, AIN2=PWM | BIN1=PWM, BIN2=0 |
| Turn Right     | AIN1=PWM, AIN2=0 | BIN1=0, BIN2=PWM |
| Stop           | AIN1=0, AIN2=0   | BIN1=0, BIN2=0   |

## Assembly Instructions

1. **Prepare the components**:
   - ESP32-S2 Mini
   - DRV8833 driver board
   - Step-up converter
   - Toggle switch or USB power switch
   - USB splitter/adapter
   - Powerbank
   - Jumper wires

2. **Prepare the power switch**:
   - Option 1: Install the USB power switch in-line with the USB cable from the powerbank
   - Option 2: Cut the red wire in a USB cable and connect the toggle switch

3. **Mount the components**:
   - Secure all components to the robot chassis
   - Position the power switch in an easily accessible location
   - Ensure motors are properly installed on the chassis

4. **Connect the power circuit**:
   - Connect the powerbank's USB output to the power switch
   - Connect the power switch output to the USB splitter
   - Connect one output from the splitter to the ESP32-S2 Mini's USB-C port
   - Extract 5V/GND from the second output and connect to the step-up converter input
   - Connect the step-up converter's 6V output to VM on the DRV8833
   - Connect all ground lines together (powerbank, ESP32-S2 Mini, step-up converter, DRV8833)

5. **Connect the control circuit**:
   - Connect GPIO7 on ESP32-S2 Mini to AIN1 on DRV8833
   - Connect GPIO8 on ESP32-S2 Mini to AIN2 on DRV8833
   - Connect GPIO9 on ESP32-S2 Mini to BIN1 on DRV8833
   - Connect GPIO10 on ESP32-S2 Mini to BIN2 on DRV8833

6. **Connect the motors**:
   - Connect left motor to AOUT1 and AOUT2 on DRV8833
   - Connect right motor to BOUT1 and BOUT2 on DRV8833

7. **Test the circuit**:
   - Power on the circuit using the global power switch
   - Verify that the ESP32-S2 Mini powers up correctly
   - Check voltage levels at the DRV8833 VM pin (should be approximately 6V)

## Advantages of This Setup

1. **True global control**: 
   - One switch powers the entire system on or off
   - No need to disconnect the powerbank when not in use
   - Prevents battery drain during storage

2. **Simplified power management**:
   - Single power source (powerbank)
   - Clean power distribution to all components
   - Separate power paths for ESP32-S2 Mini and motors after the split

3. **Enhanced development experience**:
   - Native USB support when connected to computer
   - When programming/debugging, can disconnect the powerbank and use computer USB
   - Single switch operation for field testing

4. **Compact design**:
   - ESP32-S2 Mini has a smaller footprint than ESP32-CAM
   - Better suited for small robot platforms
   - No unnecessary camera module taking up space

## Troubleshooting

### System Not Powering Up
- Check if the global power switch is in the ON position
- Verify all USB connections are secure
- Check the powerbank has sufficient charge
- Test the power switch continuity with a multimeter

### ESP32-S2 Mini Not Powering Up
- Check all USB connections in the power path
- Try a different USB cable
- Make sure the USB splitter is functioning correctly
- Verify the powerbank has sufficient charge

### Motors Not Responding
- Verify power connections (VM and GND on DRV8833)
- Ensure the step-up converter is outputting approximately 6V
- Confirm motor connections to AOUT and BOUT pins
- Verify control pin connections between ESP32-S2 Mini and DRV8833

### Motors Running In Wrong Direction
- Swap the motor terminal connections (AOUT1/AOUT2 or BOUT1/BOUT2)
- Or modify the control logic in the code to invert the direction

### Inconsistent Motor Behavior
- Check all ground connections are properly connected together
- Verify the step-up converter is providing stable voltage
- Ensure the USB splitter can provide sufficient current

### Electrical Noise Issues
- Keep signal wires away from motor wires where possible
- Add 0.1μF ceramic capacitors across motor terminals if experiencing noise issues
- Consider adding ferrite beads on motor wires

### Power Management Issues
- If powerbank auto-shuts off, look for a model with "always on" capability
- For longer run times, consider a powerbank with higher capacity (10,000mAh+)
- If the system resets when motors start, ensure USB splitter and powerbank can provide sufficient current 