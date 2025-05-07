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
  Serial.println("ESP32-S2 Mini Motor Test");
  Serial.println("************************");
  
  // Blink LED 3 times on startup
  blinkLED(3);
  
  // Setup motor control
  setupMotors();
  
  Serial.println("Starting motor test sequence...");
}

void loop() {
  // Test forward movement
  Serial.println("Moving forward (slow)");
  blinkLED(1);
  moveForward(MOTOR_SPEED_SLOW);
  delay(2000);
  
  Serial.println("Moving forward (medium)");
  blinkLED(1);
  moveForward(MOTOR_SPEED_MEDIUM);
  delay(2000);
  
  Serial.println("Moving forward (fast)");
  blinkLED(1);
  moveForward(MOTOR_SPEED_FAST);
  delay(2000);
  
  // Stop
  Serial.println("Stopping");
  blinkLED(2);
  stopMotors();
  delay(1000);
  
  // Test backward movement
  Serial.println("Moving backward (slow)");
  blinkLED(1);
  moveBackward(MOTOR_SPEED_SLOW);
  delay(2000);
  
  Serial.println("Moving backward (medium)");
  blinkLED(1);
  moveBackward(MOTOR_SPEED_MEDIUM);
  delay(2000);
  
  Serial.println("Moving backward (fast)");
  blinkLED(1);
  moveBackward(MOTOR_SPEED_FAST);
  delay(2000);
  
  // Stop
  Serial.println("Stopping");
  blinkLED(2);
  stopMotors();
  delay(1000);
  
  // Test turning
  Serial.println("Turning left");
  blinkLED(1);
  turnLeft(MOTOR_SPEED_MEDIUM);
  delay(2000);
  
  Serial.println("Turning right");
  blinkLED(1);
  turnRight(MOTOR_SPEED_MEDIUM);
  delay(2000);
  
  // Final stop
  Serial.println("Test sequence complete. Stopping.");
  blinkLED(3);
  stopMotors();
  delay(5000);  // Wait 5 seconds before repeating
} 