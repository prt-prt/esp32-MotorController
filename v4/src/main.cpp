#include <Arduino.h>
#include "motor_control.h"

#define LED_PIN 15  // Onboard LED pin

void blinkLED(int times, int duration = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(duration);
    digitalWrite(LED_PIN, LOW);
    delay(duration);
  }
}

void setup() {
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);
  delay(5000);  // Give time to open serial monitor
  
  Serial.println("\n\n");
  Serial.println("************************");
  Serial.println("Organic Movement Test");
  Serial.println("************************");
  
  // Blink LED 3 times on startup
  blinkLED(3);
  
  // Setup motor control
  setupMotors();
  
  Serial.println("Starting organic movement sequence...");
}

void loop() {
  // Get a random movement type
  MovementType movement = getRandomMovement();
  int speed = getRandomSpeed();
  int moveTime = getRandomTime(MIN_MOVE_TIME, MAX_MOVE_TIME);
  int pauseTime = getRandomTime(MIN_PAUSE_TIME, MAX_PAUSE_TIME);
  
  // Execute the movement
  switch (movement) {
    case FORWARD:
      Serial.println("Moving forward");
      moveForward(speed);
      delay(moveTime);
      break;
      
    case BACKWARD:
      Serial.println("Moving backward");
      moveBackward(speed);
      delay(moveTime);
      break;
      
    case CURVE_LEFT:
      Serial.println("Curving left");
      curveLeft(speed);
      delay(moveTime);
      break;
      
    case CURVE_RIGHT:
      Serial.println("Curving right");
      curveRight(speed);
      delay(moveTime);
      break;
      
    case PAUSE:
      Serial.println("Pausing");
      stopMotors();
      delay(pauseTime);
      break;
  }
  
  // Always stop between movements
  stopMotors();
  
  // Add a small random delay between movements
  delay(random(500, 1500));
} 