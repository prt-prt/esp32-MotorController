#include <Arduino.h>
#include "motor_control.h"

// PWM configuration
const int PWM_FREQ = 1000;  // 1kHz
const int PWM_RESOLUTION = 8;  // 8-bit resolution (0-255)

void setupMotors() {
  Serial.println("Setting up motors...");
  
  // Configure PWM
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);  // Channel 0 for Motor A IN1
  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);  // Channel 1 for Motor A IN2
  ledcSetup(2, PWM_FREQ, PWM_RESOLUTION);  // Channel 2 for Motor B IN1
  ledcSetup(3, PWM_FREQ, PWM_RESOLUTION);  // Channel 3 for Motor B IN2
  
  // Attach PWM channels to pins
  ledcAttachPin(MOTOR_A_IN1, 0);
  ledcAttachPin(MOTOR_A_IN2, 1);
  ledcAttachPin(MOTOR_B_IN1, 2);
  ledcAttachPin(MOTOR_B_IN2, 3);
  
  // Initialize motors in stopped state
  stopMotors();
  
  Serial.println("Motor setup complete");
}

void moveForward() {
  // Motor A forward
  ledcWrite(0, MOTOR_SPEED);  // IN1
  ledcWrite(1, 0);            // IN2
  
  // Motor B forward
  ledcWrite(2, MOTOR_SPEED);  // IN1
  ledcWrite(3, 0);            // IN2
}

void stopMotors() {
  // Stop both motors
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
} 