#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control pins
#define MOTOR_A_IN1 11  // GPIO11
#define MOTOR_A_IN2 9   // GPIO9
#define MOTOR_B_IN1 7   // GPIO7
#define MOTOR_B_IN2 5   // GPIO5

// Auxiliary pin
#define AUX_PIN 39    // GPIO39 for auxiliary control

// LED pin
#define LED_PIN 15    // Onboard LED

// Motor speed constant
extern const int MOTOR_SPEED_ACTUAL;

// Motor direction states
enum MotorDirection {
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
};

// Function declarations
void setupMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void setDirection(MotorDirection direction);
void toggleAuxPin();

#endif // MOTOR_CONTROL_H 