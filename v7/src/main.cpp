#include <Arduino.h>
#include "motor_control.h"
#include "movement_modes.h"

// Timing
const unsigned long BLINK_INTERVAL = 1000;      // 1 second blink interval for pin 39

// Capacitor charging delays
const unsigned long INITIAL_CAP_CHARGE_DELAY = 15000;  // 15 seconds for initial capacitor charging

// Program state
unsigned long lastBlinkTime = 0;
bool auxPinState = false;
bool initialStartupComplete = false;

void blinkLED(int times, int onTime = 200, int offTime = 200) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(onTime);
    digitalWrite(LED_PIN, LOW);
    delay(offTime);
  }
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(500); // Give serial time to initialize
  Serial.println("\n\n----- ESP32 Motor Controller v7 with Movement Modes -----");
  
  // Configure LED and AUX pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(AUX_PIN, OUTPUT);
  digitalWrite(AUX_PIN, LOW);
  
  Serial.println("Starting up - blinking LED 7 times");
  // Blink LED 7 times to indicate version 7
  blinkLED(7);
  
  Serial.println("Initializing motors");
  // Initialize motors
  setupMotors();
  
  Serial.println("Initial capacitor charging delay starting");
  Serial.print("Waiting for ");
  Serial.print(INITIAL_CAP_CHARGE_DELAY);
  Serial.println(" ms");
  
  // Initial capacitor charging delay
  // This gives time for both 2200uF capacitors to charge up
  delay(INITIAL_CAP_CHARGE_DELAY);
  Serial.println("Initial capacitor charging complete");
  
  Serial.println("Initializing movement modes");
  // Initialize movement modes
  initMovementModes();
  
  // Start with motors stopped
  setDirection(STOP);
  
  // Mark initial startup as complete
  initialStartupComplete = true;
  Serial.println("Setup complete - entering main loop");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Continuously blink AUX_PIN (GPIO39) regardless of motor state
  if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
    lastBlinkTime = currentTime;
    
    // Toggle AUX_PIN state
    auxPinState = !auxPinState;
    digitalWrite(AUX_PIN, auxPinState);
  }

  // Update current movement mode - this handles all movement patterns
  updateCurrentMode();
}