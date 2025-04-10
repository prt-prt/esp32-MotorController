#include "motor_control.h"

// L298N Motor Driver Pins for ESP32-CAM
// Motor A - Left Motor
const int IN1 = 2;    // Direction control for Motor A
const int IN2 = 12;   // Direction control for Motor A
const int ENA = 14;   // PWM enable pin for Motor A

// Motor B - Right Motor
const int IN3 = 15;   // Direction control for Motor B
const int IN4 = 13;   // Direction control for Motor B
const int ENB = 16;   // PWM enable pin for Motor B

// Motor speed constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const int DEFAULT_SPEED = 200;

// PWM properties
const int FREQ = 5000;       // PWM frequency
const int RESOLUTION = 8;    // 8-bit resolution (0-255)
const int PWM_CHANNEL_ENA = 0; // PWM channel for ENA
const int PWM_CHANNEL_ENB = 1; // PWM channel for ENB

void setupMotors() {
  // Configure PWM for enable pins
  ledcSetup(PWM_CHANNEL_ENA, FREQ, RESOLUTION);
  ledcSetup(PWM_CHANNEL_ENB, FREQ, RESOLUTION);
  
  // Attach the PWM channels to the enable pins
  ledcAttachPin(ENA, PWM_CHANNEL_ENA);
  ledcAttachPin(ENB, PWM_CHANNEL_ENB);
  
  // Initialize direction pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initially stop the motors
  stopMotors();
  
  Serial.println("Motor control initialized with L298N driver");
}

// Function to set motor A (left) speed and direction
void setMotorA(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    // Forward: IN1 HIGH, IN2 LOW
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    // Backward: IN1 LOW, IN2 HIGH
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  
  // Set the motor speed with PWM on the enable pin
  ledcWrite(PWM_CHANNEL_ENA, speed);
}

// Function to set motor B (right) speed and direction
void setMotorB(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  if (forward) {
    // Forward: IN3 HIGH, IN4 LOW
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    // Backward: IN3 LOW, IN4 HIGH
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  // Set the motor speed with PWM on the enable pin
  ledcWrite(PWM_CHANNEL_ENB, speed);
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
  // Stop by setting both direction pins to the same level (LOW)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // Stop PWM signal to the enable pins
  ledcWrite(PWM_CHANNEL_ENA, 0);
  ledcWrite(PWM_CHANNEL_ENB, 0);
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