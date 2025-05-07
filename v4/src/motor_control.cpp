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
  
  // Set fault pin as input with pullup
  pinMode(FAULT_PIN, INPUT_PULLUP);
  
  // Initialize motors in stopped state
  stopMotors();
  
  // Seed random number generator
  randomSeed(analogRead(0));
  
  Serial.println("Motor setup complete");
  printFaultStatus();
}

bool checkFault() {
  return !digitalRead(FAULT_PIN);
}

void printFaultStatus() {
  if (checkFault()) {
    Serial.println("WARNING: DRV8833 is in fault condition!");
    Serial.println("Possible causes:");
    Serial.println("1. Overcurrent protection triggered");
    Serial.println("2. Overtemperature protection triggered");
    Serial.println("3. Undervoltage lockout");
    Serial.println("4. Short circuit detected");
  } else {
    Serial.println("DRV8833 status: Normal");
  }
}

void setMotorSpeed(int channel, int speed) {
  ledcWrite(channel, speed);
  Serial.print("Channel ");
  Serial.print(channel);
  Serial.print(" speed: ");
  Serial.println(speed);
}

void rampSpeed(int startSpeed, int endSpeed, int duration) {
  int stepDelay = duration / RAMP_STEPS;
  int speedStep = (endSpeed - startSpeed) / RAMP_STEPS;
  
  for (int i = 0; i < RAMP_STEPS; i++) {
    int currentSpeed = startSpeed + (speedStep * i);
    setMotorSpeed(0, currentSpeed);
    setMotorSpeed(2, currentSpeed);
    delay(stepDelay);
    
    if (checkFault()) {
      Serial.println("Fault detected during speed ramping!");
      printFaultStatus();
      stopMotors();
      delay(1000);
      return;
    }
  }
}

void moveForward(int speed) {
  Serial.print("Moving forward at speed: ");
  Serial.println(speed);
  
  if (checkFault()) {
    Serial.println("Cannot move forward - fault detected!");
    printFaultStatus();
    return;
  }
  
  // Ramp up to speed
  rampSpeed(0, speed, RAMP_DELAY * RAMP_STEPS);
  
  // Motor A forward
  setMotorSpeed(0, speed);  // IN1
  setMotorSpeed(1, 0);      // IN2
  
  // Motor B forward
  setMotorSpeed(2, speed);  // IN1
  setMotorSpeed(3, 0);      // IN2
  
  if (checkFault()) {
    Serial.println("Fault detected after setting forward speed!");
    printFaultStatus();
    stopMotors();
  }
}

void moveBackward(int speed) {
  Serial.print("Moving backward at speed: ");
  Serial.println(speed);
  
  if (checkFault()) {
    Serial.println("Cannot move backward - fault detected!");
    printFaultStatus();
    return;
  }
  
  // Ramp up to speed
  rampSpeed(0, speed, RAMP_DELAY * RAMP_STEPS);
  
  // Motor A backward
  setMotorSpeed(0, 0);      // IN1
  setMotorSpeed(1, speed);  // IN2
  
  // Motor B backward
  setMotorSpeed(2, 0);      // IN1
  setMotorSpeed(3, speed);  // IN2
  
  if (checkFault()) {
    Serial.println("Fault detected after setting backward speed!");
    printFaultStatus();
    stopMotors();
  }
}

void curveLeft(int baseSpeed) {
  Serial.print("Curving left at base speed: ");
  Serial.println(baseSpeed);
  
  if (checkFault()) {
    Serial.println("Cannot curve left - fault detected!");
    printFaultStatus();
    return;
  }
  
  int leftSpeed = max(baseSpeed - CURVE_SPEED_DIFF, MIN_SPEED);
  int rightSpeed = min(baseSpeed + CURVE_SPEED_DIFF, MAX_SPEED);
  
  Serial.print("Left motor speed: ");
  Serial.print(leftSpeed);
  Serial.print(", Right motor speed: ");
  Serial.println(rightSpeed);
  
  // Ramp up to speed
  rampSpeed(0, baseSpeed, RAMP_DELAY * RAMP_STEPS);
  
  // Motor A forward (left)
  setMotorSpeed(0, leftSpeed);  // IN1
  setMotorSpeed(1, 0);          // IN2
  
  // Motor B forward (right)
  setMotorSpeed(2, rightSpeed); // IN1
  setMotorSpeed(3, 0);          // IN2
  
  if (checkFault()) {
    Serial.println("Fault detected after setting curve left speed!");
    printFaultStatus();
    stopMotors();
  }
}

void curveRight(int baseSpeed) {
  Serial.print("Curving right at base speed: ");
  Serial.println(baseSpeed);
  
  if (checkFault()) {
    Serial.println("Cannot curve right - fault detected!");
    printFaultStatus();
    return;
  }
  
  int leftSpeed = min(baseSpeed + CURVE_SPEED_DIFF, MAX_SPEED);
  int rightSpeed = max(baseSpeed - CURVE_SPEED_DIFF, MIN_SPEED);
  
  Serial.print("Left motor speed: ");
  Serial.print(leftSpeed);
  Serial.print(", Right motor speed: ");
  Serial.println(rightSpeed);
  
  // Ramp up to speed
  rampSpeed(0, baseSpeed, RAMP_DELAY * RAMP_STEPS);
  
  // Motor A forward (left)
  setMotorSpeed(0, leftSpeed);  // IN1
  setMotorSpeed(1, 0);          // IN2
  
  // Motor B forward (right)
  setMotorSpeed(2, rightSpeed); // IN1
  setMotorSpeed(3, 0);          // IN2
  
  if (checkFault()) {
    Serial.println("Fault detected after setting curve right speed!");
    printFaultStatus();
    stopMotors();
  }
}

void stopMotors() {
  Serial.println("Stopping motors");
  
  // Ramp down to stop
  rampSpeed(ledcRead(0), 0, RAMP_DELAY * RAMP_STEPS);
  
  // Stop both motors
  setMotorSpeed(0, 0);
  setMotorSpeed(1, 0);
  setMotorSpeed(2, 0);
  setMotorSpeed(3, 0);
  
  if (checkFault()) {
    Serial.println("Fault detected after stopping motors!");
    printFaultStatus();
  }
}

int getRandomSpeed() {
  return random(MIN_SPEED, MAX_SPEED + 1);
}

int getRandomTime(int minTime, int maxTime) {
  return random(minTime, maxTime + 1);
}

MovementType getRandomMovement() {
  int choice = random(100);
  
  if (choice < 30) return FORWARD;      // 30% chance
  if (choice < 50) return CURVE_LEFT;   // 20% chance
  if (choice < 70) return CURVE_RIGHT;  // 20% chance
  if (choice < 85) return BACKWARD;     // 15% chance
  return PAUSE;                         // 15% chance
} 