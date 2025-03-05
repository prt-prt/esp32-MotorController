#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// DRV8833 Motor Driver Pins
// Motor A - Left Motor
extern const int AIN1; // PWM control for Motor A direction 1
extern const int AIN2; // PWM control for Motor A direction 2

// Motor B - Right Motor
extern const int BIN1; // PWM control for Motor B direction 1
extern const int BIN2; // PWM control for Motor B direction 2

// Motor speed constants
extern const int MAX_SPEED;
extern const int MIN_SPEED;
extern const int DEFAULT_SPEED;

// PWM properties
extern const int FREQ;           // PWM frequency
extern const int RESOLUTION;     // 8-bit resolution (0-255)
extern const int PWM_CHANNEL_AIN1;
extern const int PWM_CHANNEL_AIN2;
extern const int PWM_CHANNEL_BIN1;
extern const int PWM_CHANNEL_BIN2;

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