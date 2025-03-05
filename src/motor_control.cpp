#include "motor_control.h"

// L298N Motor Driver Pins for ESP32-CAM8
// Motor A - Left Motor
const int IN1 = 12; // GPIO12 - Available on ESP32-CAM
const int IN2 = 13; // GPIO13 - Available on ESP32-CAM

// Motor B - Right Motor
const int IN3 = 14; // GPIO14 - Available on ESP32-CAM
const int IN4 = 15;  // GPIO15 - Available on ESP32-CAM

// Motor speed constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const int DEFAULT_SPEED = 200;

void setupMotors() {
  // Set all the motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initially stop the motors
  stopMotors();
}

// Function to move the robot forward
void moveForward(int speed) {
  // Speed parameter is kept for API compatibility but not used
  
  // Set Motor A forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Set Motor B forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to move the robot backward
void moveBackward(int speed) {
  // Speed parameter is kept for API compatibility but not used
  
  // Set Motor A backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Set Motor B backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to turn the robot left
void turnLeft(int speed) {
  // Speed parameter is kept for API compatibility but not used
  
  // Set Motor A backward (or stop)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Set Motor B forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn the robot right
void turnRight(int speed) {
  // Speed parameter is kept for API compatibility but not used
  
  // Set Motor A forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Set Motor B backward (or stop)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to stop the motors
void stopMotors() {
  // Stop Motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  // Stop Motor B
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
} 