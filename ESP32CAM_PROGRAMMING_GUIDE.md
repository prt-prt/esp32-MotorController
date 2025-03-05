# ESP32-CAM Programming Guide

This guide provides detailed instructions for programming the ESP32-CAM board for the robot car project. The ESP32-CAM has specific requirements for programming that differ from standard ESP32 boards.

## Hardware Requirements

- ESP32-CAM board
- FTDI USB-to-TTL Serial Adapter (3.3V)
- Jumper wires
- Breadboard (optional, but helpful)
- Push button or jumper wire for reset/programming

## Connection Setup

The ESP32-CAM doesn't have a built-in USB port for programming. You need to use an FTDI adapter (or similar USB-to-TTL converter) with the following connections:

| FTDI Adapter | ESP32-CAM |
|--------------|-----------|
| 5V/VCC       | 5V        |
| GND          | GND       |
| TX           | RX (GPIO3) |
| RX           | TX (GPIO1) |

**IMPORTANT**: During upload, you need to connect GPIO0 to GND to put the ESP32-CAM in bootloader mode. This can be done by:
1. Adding a push button between GPIO0 and GND
2. Using a jumper wire to temporarily connect GPIO0 to GND during upload

## Programming Steps

1. **Make the connections** as shown in the table above.

2. **Enter bootloader mode**:
   - Connect GPIO0 to GND
   - Press and release the RST (reset) button on the ESP32-CAM
   - Keep GPIO0 connected to GND

3. **Upload the code**:
   - Start the upload from PlatformIO
   - Wait until you see "Connecting........_____" in the terminal
   - If the upload fails with timeout errors, retry the reset sequence

4. **Exit bootloader mode**:
   - Disconnect GPIO0 from GND
   - Press the RST button again to restart the ESP32-CAM with the new firmware

## Common Problems and Solutions

### "Unable to verify flash chip connection" or "Serial data stream stopped"

This usually indicates issues with the bootloader mode. Try:

1. **Lower upload speed**: 
   - Open `platformio.ini`
   - Set `upload_speed = 115200` (already done in our project)

2. **Check connections**:
   - Ensure TX from FTDI goes to RX of ESP32-CAM (GPIO3)
   - Ensure RX from FTDI goes to TX of ESP32-CAM (GPIO1)
   - Verify you have a common GND between FTDI and ESP32-CAM

3. **Timing of reset sequence**:
   - Sometimes the timing matters. Try this sequence:
     1. Connect GPIO0 to GND
     2. Press and hold RST button
     3. Release RST button
     4. Start upload immediately
     5. If upload starts (shows connecting), you're good

4. **Add capacitor**:
   - Adding a 10μF capacitor between EN/RST and GND can help with auto-reset
   - Use the capacitor's negative leg to GND and positive to RST

### "Error: A fatal error occurred: Failed to connect to ESP32"

1. **Power issues**:
   - Ensure FTDI adapter can provide enough power
   - Try an external 5V power supply for the ESP32-CAM

2. **Hardware issues**:
   - Some ESP32-CAM modules may have different pin layouts
   - Double-check the pinout for your specific model

## Verifying Success

After uploading:

1. Open the **Serial Monitor** at 115200 baud
2. Reset the ESP32-CAM by pressing its RST button
3. You should see the startup messages including:
   ```
   ESP32-CAM Robot Car Control - Demo Mode
   ************************
   Board: ESP32-CAM
   Pins configured for ESP32-CAM:
   Motor A: ENA=GPIO2, IN1=GPIO14, IN2=GPIO15
   Motor B: ENB=GPIO13, IN3=GPIO12, IN4=GPIO4
   Status LED: GPIO33
   Setup complete, starting demo sequence...
   ```

4. The onboard red LED (GPIO33) should blink during startup and operation

## Debug Mode

If you're having trouble with the motors or want to test just the ESP32-CAM connection:

1. Edit `src/main.cpp`
2. Uncomment the line `// #define DEBUG_MODE`
3. Upload the modified code
4. This will disable motor movements and only blink the LED

## Additional Resources

- [ESP32-CAM Pinout Reference](https://randomnerdtutorials.com/esp32-cam-ai-thinker-pinout/)
- [Detailed ESP32-CAM Programming Tutorial](https://randomnerdtutorials.com/program-upload-code-esp32-cam/)
- [Troubleshooting ESP32 Connection Issues](https://randomnerdtutorials.com/esp32-troubleshooting-guide/) 