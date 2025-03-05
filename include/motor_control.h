#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// L298N Motor Driver Pins
// Motor A - Left Motor
extern const int ENA; // PWM pin for controlling speed of motor A
extern const int IN1; // Control pin 1 for motor A
extern const int IN2; // Control pin 2 for motor A

// Motor B - Right Motor
extern const int ENB; // PWM pin for controlling speed of motor B
extern const int IN3; // Control pin 1 for motor B
extern const int IN4; // Control pin 2 for motor B

// Motor speed constants
extern const int MAX_SPEED;
extern const int MIN_SPEED;
extern const int DEFAULT_SPEED;

// PWM properties
extern const int FREQ;       // PWM frequency
extern const int RESOLUTION; // 8-bit resolution (0-255)
extern const int PWM_CHANNEL_A;
extern const int PWM_CHANNEL_B;

// Motor control functions
void setupMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif // MOTOR_CONTROL_H 