#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control pins
#define MOTOR_A_IN1 11  // GPIO11
#define MOTOR_A_IN2 9   // GPIO9
#define MOTOR_B_IN1 7   // GPIO7
#define MOTOR_B_IN2 5   // GPIO5

// Motor speed (0-255)
#define MOTOR_SPEED 128  // 50% duty cycle

// Function declarations
void setupMotors();
void moveForward();
void stopMotors();

#endif // MOTOR_CONTROL_H 