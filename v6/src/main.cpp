#include <Arduino.h>
#include "motor_control.h"

// Timing
const unsigned long DIRECTION_CHANGE_INTERVAL = 5000;  // 5 seconds between direction changes
const unsigned long AUX_PIN_TOGGLE_INTERVAL = 500;    // 5 seconds for aux pin toggle

unsigned long lastDirectionChange = 0;
unsigned long lastAuxPinToggle = 0;
int currentDirection = 0;

void blinkLED(int times, int onTime = 200, int offTime = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(onTime);
    digitalWrite(LED_PIN, LOW);
    delay(offTime);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give time for serial monitor to connect
  
  // Configure LED pin
  pinMode(LED_PIN, OUTPUT);
  
  // Blink LED 6 times to indicate version 6
  blinkLED(6);
  
  Serial.println("\nESP32-S2 Motor Test v6");
  Serial.println("----------------------");
  Serial.println("Testing all directions & GPIO39 toggle");
  
  setupMotors();
  
  // Start with motors moving forward
  setDirection(FORWARD);
  lastDirectionChange = millis();
  
  // Initialize aux pin state
  lastAuxPinToggle = millis();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check if it's time to change direction and we haven't gone through all directions
  if ((currentTime - lastDirectionChange >= DIRECTION_CHANGE_INTERVAL) && (currentDirection < 4)) {
    lastDirectionChange = currentTime;
    
    // Cycle through directions: FORWARD -> BACKWARD -> LEFT -> RIGHT -> STOP
    setDirection(static_cast<MotorDirection>(currentDirection));
    currentDirection++;
    
    // After going through all directions, stop the motors
    if (currentDirection == 4) {
      setDirection(STOP);
    }
  }
  
  // Check if it's time to toggle auxiliary pin
  if (currentTime - lastAuxPinToggle >= AUX_PIN_TOGGLE_INTERVAL) {
    lastAuxPinToggle = currentTime;
    toggleAuxPin();
  }
}