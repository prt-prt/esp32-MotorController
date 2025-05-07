#include <Arduino.h>
#include "motor_control.h"
#include "movement_modes.h"
#include "test_mode.h"
#include "wander_mode.h"
#include "rotate_mode.h"

// Current movement mode
MovementMode currentMode = WANDER_MODE;

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

// Wander mode - randomized organic movement with varying speeds
void runWanderMode() {
  // Select a movement pattern (more complex with 8 options)
  int movementType = random(8); // 0-7
  
  // Base speed and speed difference for curves
  int baseSpeed = random(MIN_SPEED + 50, MAX_SPEED);
  int speedDiff = random(20, 100);  // Difference between wheels for curves
  int duration = random(500, 3000); // Movement duration
  
  // Variables for differential drive
  int leftSpeed = 0;
  int rightSpeed = 0;
  
  switch (movementType) {
    case 0: // Straight forward
      Serial.print("Wander: Forward at speed ");
      Serial.println(baseSpeed);
      moveForward(baseSpeed);
      break;
      
    case 1: // Gentle left curve
      leftSpeed = baseSpeed - speedDiff;
      rightSpeed = baseSpeed;
      Serial.print("Wander: Gentle left curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 2: // Gentle right curve
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed - speedDiff;
      Serial.print("Wander: Gentle right curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 3: // Sharp left curve
      leftSpeed = baseSpeed - speedDiff*1.5;
      rightSpeed = baseSpeed;
      Serial.print("Wander: Sharp left curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 4: // Sharp right curve
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed - speedDiff*1.5;
      Serial.print("Wander: Sharp right curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 5: // Spin turn - traditional on-spot turn
      if (random(2) == 0) { // Randomly choose direction
        Serial.print("Wander: Spin left at speed ");
        Serial.println(baseSpeed);
        turnLeft(baseSpeed);
      } else {
        Serial.print("Wander: Spin right at speed ");
        Serial.println(baseSpeed);
        turnRight(baseSpeed);
      }
      break;
      
    case 6: // Backward move
      Serial.print("Wander: Backward at speed ");
      Serial.println(baseSpeed);
      moveBackward(baseSpeed);
      break;
      
    case 7: // Stop for a moment
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