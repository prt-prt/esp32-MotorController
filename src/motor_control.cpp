#include "motor_control.h"

// L298N Motor Driver Pins for ESP32-CAM
// Motor A - Left Motor
const int ENA = 2;  // PWM pin for Motor A - GPIO2 available on ESP32-CAM
const int IN1 = 12; // GPIO12 - Available on ESP32-CAM
const int IN2 = 13; // GPIO13 - Available on ESP32-CAM

// Motor B - Right Motor
const int ENB = 15; // PWM pin for Motor B - GPIO15 available on ESP32-CAM
const int IN3 = 14; // GPIO14 - Available on ESP32-CAM
const int IN4 = 0;  // GPIO0 - Available on ESP32-CAM (changed from 15 to avoid conflict with ENB)

// Motor speed constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const int DEFAULT_SPEED = 200;

// PWM properties
const int FREQ = 5000;       // PWM frequency
const int RESOLUTION = 8;    // 8-bit resolution (0-255)
const int PWM_CHANNEL_A = 0; // PWM channel for Motor A
const int PWM_CHANNEL_B = 1; // PWM channel for Motor B

void setupMotors() {
  // Set all the motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Configure PWM for the enable pins
  ledcSetup(PWM_CHANNEL_A, FREQ, RESOLUTION);
  ledcSetup(PWM_CHANNEL_B, FREQ, RESOLUTION);
  
  // Attach the PWM channels to the enable pins
  ledcAttachPin(ENA, PWM_CHANNEL_A);
  ledcAttachPin(ENB, PWM_CHANNEL_B);
  
  // Initially stop the motors
  stopMotors();
  
  Serial.println("Motor control initialized with PWM speed control.");
}

// Function to set motor A (left) speed and direction
void setMotorA(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  // Set direction
  if (forward) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  
  // Set speed using PWM
  ledcWrite(PWM_CHANNEL_A, speed);
}

// Function to set motor B (right) speed and direction
void setMotorB(int speed, bool forward) {
  // Constrain speed to valid range
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  
  // Set direction
  if (forward) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  
  // Set speed using PWM
  ledcWrite(PWM_CHANNEL_B, speed);
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
  // Stop both motors by setting speed to 0
  setMotorA(0, true);
  setMotorB(0, true);
  
  // Additionally, set the direction pins to LOW to ensure the motors are off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
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