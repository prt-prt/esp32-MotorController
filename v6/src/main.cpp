#include <Arduino.h>
#include "motor_control.h"

// Timing
const unsigned long DIRECTION_TEST_TIME = 1500;  // 1.5 seconds per direction
const unsigned long BLINK_INTERVAL = 1000;      // 1 second blink interval for pin 39
const unsigned long BETWEEN_TESTS_DELAY = 1000; // 1 second between direction tests

// Program state
unsigned long lastStateChange = 0;
unsigned long lastBlinkTime = 0;
int currentState = 0;
bool auxPinState = false;
bool motorTestComplete = false;

void blinkLED(int times, int onTime = 200, int offTime = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(onTime);
    digitalWrite(LED_PIN, LOW);
    delay(offTime);
  }
}

void setup() {
  // Configure LED and AUX pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(AUX_PIN, OUTPUT);
  digitalWrite(AUX_PIN, LOW);
  
  // Blink LED 6 times to indicate version 6
  blinkLED(6);
  
  delay(8000);

  // Initialize motors
  setupMotors();
  
  // Initialize state machine
  lastStateChange = millis();
  lastBlinkTime = millis();
  
  // Start with motors stopped
  setDirection(STOP);
  
}

void loop() {
  unsigned long currentTime = millis();
  
  // PART 1: Simple motor direction test sequence
  if (!motorTestComplete) {
    if (currentTime - lastStateChange >= DIRECTION_TEST_TIME) {
      lastStateChange = currentTime;
      
      switch (currentState) {
        case 0:
          // Forward
          setDirection(FORWARD);
          break;
        case 1:
          // Stop
          setDirection(STOP);
          delay(BETWEEN_TESTS_DELAY);
          break;
        case 2:
          // Backward
          setDirection(BACKWARD);
          break;
        case 3:
          // Stop
          setDirection(STOP);
          delay(BETWEEN_TESTS_DELAY);
          break;
        case 4:
          // Left
          setDirection(TURN_LEFT);
          break;
        case 5:
          // Stop
          setDirection(STOP);
          delay(BETWEEN_TESTS_DELAY);
          break;
        case 6:
          // Right
          setDirection(TURN_RIGHT);
          break;
        case 7:
          // Final stop
          setDirection(STOP);
          delay(BETWEEN_TESTS_DELAY);
          
          // Loop back to the beginning
          currentState = -1; // Will increment to 0
          break;
      }
      
      currentState++;
    }
  }
  
  // PART 2: Continuously blink AUX_PIN (GPIO39) regardless of motor state
  if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
    lastBlinkTime = currentTime;
    
    // Toggle AUX_PIN state
    auxPinState = !auxPinState;
    digitalWrite(AUX_PIN, auxPinState);
  }
}