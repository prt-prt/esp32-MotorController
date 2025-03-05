#include "test_mode.h"
#include "motor_control.h"

// Test mode - original functionality for testing basic motor operations
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