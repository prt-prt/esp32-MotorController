#include "motor_control.h"

// DRV8833 Motor Driver Pins for ESP32-CAM
// Motor A - Left Motor
const int AIN1 = 39;  // PWM control for Motor A direction 1
const int AIN2 = 37; // PWM control for Motor A direction 2

// Motor B - Right Motor
const int BIN1 = 35; // PWM control for Motor B direction 1
const int BIN2 = 33; // PWM control for Motor B direction 2

// Motor speed constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const int DEFAULT_SPEED = 200;

// PWM properties
const int FREQ = 5000;        // PWM frequency
const int RESOLUTION = 8;     // 8-bit resolution (0-255)
const int PWM_CHANNEL_AIN1 = 0; // PWM channel for AIN1
const int PWM_CHANNEL_AIN2 = 1; // PWM channel for AIN2
const int PWM_CHANNEL_BIN1 = 2; // PWM channel for BIN1
const int PWM_CHANNEL_BIN2 = 3; // PWM channel for BIN2

void setupMotors() {
  // Configure PWM for all motor control pins
  ledcSetup(PWM_CHANNEL_AIN1, FREQ, RESOLUTION);
  ledcSetup(PWM_CHANNEL_AIN2, FREQ, RESOLUTION);
  ledcSetup(PWM_CHANNEL_BIN1, FREQ, RESOLUTION);
  ledcSetup(PWM_CHANNEL_BIN2, FREQ, RESOLUTION);
  
  // Attach the PWM channels to the pins
  ledcAttachPin(AIN1, PWM_CHANNEL_AIN1);
  ledcAttachPin(AIN2, PWM_CHANNEL_AIN2);
  ledcAttachPin(BIN1, PWM_CHANNEL_BIN1);
  ledcAttachPin(BIN2, PWM_CHANNEL_BIN2);
  
  // Initially stop the motors
  stopMotors();
  
  Serial.println("Motor control initialized with DRV8833 driver (v2)");
}

// Function to set motor A (left) speed and direction
void setMotorA(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    // Forward: PWM on AIN1, 0 on AIN2
    ledcWrite(PWM_CHANNEL_AIN1, speed);
    ledcWrite(PWM_CHANNEL_AIN2, 0);
  } else {
    // Backward: 0 on AIN1, PWM on AIN2
    ledcWrite(PWM_CHANNEL_AIN1, 0);
    ledcWrite(PWM_CHANNEL_AIN2, speed);
  }
}

// Function to set motor B (right) speed and direction
void setMotorB(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    // Forward: PWM on BIN1, 0 on BIN2
    ledcWrite(PWM_CHANNEL_BIN1, speed);
    ledcWrite(PWM_CHANNEL_BIN2, 0);
  } else {
    // Backward: 0 on BIN1, PWM on BIN2
    ledcWrite(PWM_CHANNEL_BIN1, 0);
    ledcWrite(PWM_CHANNEL_BIN2, speed);
  }
}

// Function to move the robot forward
void moveForward(int speed) {
  // Set both motors to forward direction with the same speed
  setMotorA(speed, true);
  setMotorB(speed, true);
}

// Function to move the robot backward
void moveBackward(int speed) {
  // Set both motors to backward direction with the same speed
  setMotorA(speed, false);
  setMotorB(speed, false);
}

// Function to turn the robot left
void turnLeft(int speed) {
  // Rotate left by moving right motor forward and left motor backward
  setMotorA(speed, false);
  setMotorB(speed, true);
}

// Function to turn the robot right
void turnRight(int speed) {
  // Rotate right by moving left motor forward and right motor backward
  setMotorA(speed, true);
  setMotorB(speed, false);
}

// Function to stop the motors
void stopMotors() {
  // Using brake mode (both pins LOW for coast or both HIGH for brake)
  // We'll use coast mode (both LOW) for smoother stops
  ledcWrite(PWM_CHANNEL_AIN1, 0);
  ledcWrite(PWM_CHANNEL_AIN2, 0);
  ledcWrite(PWM_CHANNEL_BIN1, 0);
  ledcWrite(PWM_CHANNEL_BIN2, 0);
}

// NEW FUNCTIONS FOR DIFFERENTIAL STEERING

// Function to curve left (both motors forward, but right faster than left)
void curveLeft(int leftSpeed, int rightSpeed) {
  setMotorA(leftSpeed, true);   // Left motor
  setMotorB(rightSpeed, true);  // Right motor
}

// Function to curve right (both motors forward, but left faster than right)
void curveRight(int leftSpeed, int rightSpeed) {
  setMotorA(leftSpeed, true);   // Left motor
  setMotorB(rightSpeed, true);  // Right motor
}

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