#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Motor control pins for DRV8833
#define MOTOR_A_IN1 11  // GPIO11
#define MOTOR_A_IN2 9   // GPIO9
#define MOTOR_B_IN1 7   // GPIO7
#define MOTOR_B_IN2 5   // GPIO5
#define FAULT_PIN 12    // GPIO12 for nFAULT pin

// Movement parameters
#define MIN_SPEED 10    // Minimum speed to prevent stalling
#define MAX_SPEED 150    // Maximum speed for organic movement
#define CURVE_SPEED_DIFF 5   // Reduced speed difference between motors
#define RAMP_STEPS 10   // Number of steps for speed ramping
#define RAMP_DELAY 50   // Delay between ramp steps (ms)

// Timing parameters
#define MIN_MOVE_TIME 2000   // Minimum time to move in one direction (ms)
#define MAX_MOVE_TIME 5000   // Maximum time to move in one direction (ms)
#define MIN_PAUSE_TIME 3000  // Minimum pause time between movements (ms)
#define MAX_PAUSE_TIME 8000  // Maximum pause time between movements (ms)

// Movement types
enum MovementType {
  FORWARD,
  BACKWARD,
  CURVE_LEFT,
  CURVE_RIGHT,
  PAUSE
};

// Function declarations
void setupMotors();
void moveForward(int speed);
void moveBackward(int speed);
void curveLeft(int baseSpeed);
void curveRight(int baseSpeed);
void stopMotors();
void rampSpeed(int startSpeed, int endSpeed, int duration);
int getRandomSpeed();
int getRandomTime(int minTime, int maxTime);
MovementType getRandomMovement();
bool checkFault();
void printFaultStatus();

#endif // MOTOR_CONTROL_H 