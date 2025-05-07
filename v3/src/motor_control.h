#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control pins for DRV8833
#define MOTOR_A_IN1 11  // GPIO1
#define MOTOR_A_IN2 9  // GPIO2
#define MOTOR_B_IN1 7  // GPIO3
#define MOTOR_B_IN2 5  // GPIO4

// Motor speeds
#define MOTOR_SPEED_SLOW 100
#define MOTOR_SPEED_MEDIUM 150
#define MOTOR_SPEED_FAST 200

// Function declarations
void setupMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

#endif // MOTOR_CONTROL_H 