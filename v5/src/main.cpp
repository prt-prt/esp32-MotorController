#include <Arduino.h>
#include "motor_control.h"

// Define the onboard LED pin for ESP32-S2
#define LED_PIN 15

void blinkLED(int times, int onTime = 200, int offTime = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(onTime);
    digitalWrite(LED_PIN, LOW);
    delay(offTime);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give time for serial monitor to connect
  
  // Configure LED pin
  pinMode(LED_PIN, OUTPUT);
  
  // Blink LED 5 times to indicate version 5
  blinkLED(5);
  
  Serial.println("\nESP32-S2 Motor Test v5");
  Serial.println("----------------------");
  
  setupMotors();
  Serial.println("Starting continuous forward movement...");
  moveForward();  // Start motors and keep them running
}

void loop() {
  // Empty loop - motors will continue running
  delay(1000);  // Small delay to prevent watchdog timer issues
} 