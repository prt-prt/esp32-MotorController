#include <Arduino.h>
#include "motor_control.h"

// Movement modes
enum MovementMode {
  TEST_MODE,
  WANDER_MODE,
  ROTATE_MODE
};

// Current movement mode
MovementMode currentMode = WANDER_MODE;

// Function prototypes
void runTestMode();
void runWanderMode();
void runRotateMode();

void setup() {
  Serial.begin(115200);
  delay(5000);
  
  Serial.println("\n\n");
  Serial.println("************************");
  Serial.println("ESP32-CAM Robot Car");
  Serial.println("************************");

  // Setup motor control
  setupMotors();
  
  // Seed random number generator
  randomSeed(analogRead(0));
  
  Serial.println("Starting in WANDER mode");
  Serial.println("------------------------");
}

void loop() {
  // Run the appropriate mode
  switch (currentMode) {
    case TEST_MODE:
      runTestMode();
      break;
    case WANDER_MODE:
      runWanderMode();
      break;
    case ROTATE_MODE:
      runRotateMode();
      break;
  }
  
  // Mode switching logic (can be expanded with button input or remote control)
  // For now, let's switch modes every 30 seconds
  static unsigned long lastModeSwitch = 0;
  if (millis() - lastModeSwitch > 30000) {
    lastModeSwitch = millis();
    
    // Toggle between WANDER_MODE and ROTATE_MODE
    if (currentMode == WANDER_MODE) {
      currentMode = ROTATE_MODE;
      Serial.println("\nSwitching to ROTATE mode");
      Serial.println("------------------------");
    } else {
      currentMode = WANDER_MODE;
      Serial.println("\nSwitching to WANDER mode");
      Serial.println("------------------------");
    }
  }
}

// Original test mode logic
void runTestMode() {
  // Test forward
  Serial.println("Testing motors: FORWARD");
  moveForward(DEFAULT_SPEED);
  delay(2000);
  
  // Test stop
  Serial.println("Testing motors: STOP");
  stopMotors();
  delay(1000);
  
  // Test backward
  Serial.println("Testing motors: BACKWARD");
  moveBackward(DEFAULT_SPEED);
  delay(2000);
  
  // Test stop
  Serial.println("Testing motors: STOP");
  stopMotors();
  delay(1000);
  
  // Test left turn
  Serial.println("Testing motors: LEFT TURN");
  turnLeft(DEFAULT_SPEED);
  delay(2000);
  
  // Test stop
  Serial.println("Testing motors: STOP");
  stopMotors();
  delay(1000);
  
  // Test right turn
  Serial.println("Testing motors: RIGHT TURN");
  turnRight(DEFAULT_SPEED);
  delay(2000);
  
  // Test stop
  Serial.println("Testing motors: STOP");
  stopMotors();
  delay(1000);
}

// Wander mode - random movement with varying speeds and pauses
void runWanderMode() {
  // Random movement selection
  int movement = random(5); // 0-4
  int speed = random(MIN_SPEED + 50, MAX_SPEED);
  int duration = random(500, 3000); // Movement duration between 0.5-3 seconds
  
  switch (movement) {
    case 0: // Forward
      Serial.print("Wander: Forward at speed ");
      Serial.println(speed);
      moveForward(speed);
      break;
    case 1: // Backward
      Serial.print("Wander: Backward at speed ");
      Serial.println(speed);
      moveBackward(speed);
      break;
    case 2: // Left turn
      Serial.print("Wander: Left turn at speed ");
      Serial.println(speed);
      turnLeft(speed);
      break;
    case 3: // Right turn
      Serial.print("Wander: Right turn at speed ");
      Serial.println(speed);
      turnRight(speed);
      break;
    case 4: // Stop
      Serial.println("Wander: Stopping for a moment");
      stopMotors();
      duration = random(1000, 5000); // Stop for 1-5 seconds
      break;
  }
  
  delay(duration);
}

// Rotate mode - slow rotation around its own axis
void runRotateMode() {
  // Slow rotation speed (30% of max speed)
  int rotationSpeed = MAX_SPEED * 0.3;
  
  // Choose a random rotation direction, but change it less frequently
  static int rotationDirection = 0;
  static unsigned long lastDirectionChange = 0;
  
  if (millis() - lastDirectionChange > 10000 || lastDirectionChange == 0) { // Change every 10 seconds
    lastDirectionChange = millis();
    rotationDirection = random(2); // 0 or 1
    
    if (rotationDirection == 0) {
      Serial.print("Rotate: Turning left at speed ");
      Serial.println(rotationSpeed);
    } else {
      Serial.print("Rotate: Turning right at speed ");
      Serial.println(rotationSpeed);
    }
  }
  
  // Perform the rotation
  if (rotationDirection == 0) {
    turnLeft(rotationSpeed);
  } else {
    turnRight(rotationSpeed);
  }
  
  // Small delay to prevent too frequent serial prints
  delay(1000);
} 