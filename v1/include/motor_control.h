#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// L298N Motor Driver Pins
// Motor A - Left Motor
extern const int IN1;  // Direction control for Motor A
extern const int IN2;  // Direction control for Motor A
extern const int ENA;  // PWM enable pin for Motor A

// Motor B - Right Motor
extern const int IN3;  // Direction control for Motor B
extern const int IN4;  // Direction control for Motor B
extern const int ENB;  // PWM enable pin for Motor B

// Motor speed constants
extern const int MAX_SPEED;
extern const int MIN_SPEED;
extern const int DEFAULT_SPEED;

// PWM properties
extern const int FREQ;          // PWM frequency
extern const int RESOLUTION;    // 8-bit resolution (0-255)
extern const int PWM_CHANNEL_ENA;
extern const int PWM_CHANNEL_ENB;

// Basic motor control functions
void setupMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

// Low-level motor control functions
void setMotorA(int speed, bool forward);
void setMotorB(int speed, bool forward);

// Differential steering functions for organic movement
void curveLeft(int leftSpeed, int rightSpeed);
void curveRight(int leftSpeed, int rightSpeed);
void moveDifferential(int leftSpeed, int rightSpeed);

#endif // MOTOR_CONTROL_H 