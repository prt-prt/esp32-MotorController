#include <Arduino.h>
#include "movement_modes.h"
#include "motor_control.h"

// Global state
static MovementMode* currentMode = nullptr;
static int currentModeIndex = 0;
static int currentDurationIndex = 0;
static unsigned long modeStartTime = 0;
static unsigned long lastMovementUpdate = 0;
static unsigned long lastAuxPinUpdate = 0;
static bool auxPinState = false;
static bool inRestPeriod = false;
static int restDuration = 0;

// Define all movement modes
static MovementMode movementModes[] = {
  {
    "Spin",
    spinPattern,
    auxPinBlink,
    100,    // Update movement every 100ms
    100,    // Update aux pin every 100ms
    100     // Keep aux pin on for 100ms
  },
  {
    "Wander",
    wanderPattern,
    auxPinRandom,
    500,    // Update movement every 500ms
    200,    // Update aux pin every 200ms
    50      // Keep aux pin on for 50ms
  },
  {
    "Pulse",
    pulsePattern,
    auxPinPulse,
    1000,   // Update movement every 1000ms
    500,    // Update aux pin every 500ms
    200     // Keep aux pin on for 200ms
  },
  {
    "Circle",
    circlePattern,
    auxPinWave,
    200,    // Update movement every 200ms
    100,    // Update aux pin every 100ms
    150     // Keep aux pin on for 150ms
  },
  {
    "Zigzag",
    zigzagPattern,
    auxPinBlink,
    300,    // Update movement every 300ms
    150,    // Update aux pin every 150ms
    75      // Keep aux pin on for 75ms
  },
  {
    "Stop",
    stopPattern,
    auxPinOff,
    1000,   // Update movement every 1000ms
    1000,   // Update aux pin every 1000ms
    0       // Keep aux pin off
  },
  {
    "Rest",
    restPattern,
    auxPinOff,
    2000,   // Update movement every 2000ms (just to check status)
    2000,   // Update aux pin every 2000ms
    0       // Keep aux pin off
  }
};

int getRandomRestDuration() {
  return random(MIN_REST_DURATION, MAX_REST_DURATION + 1);
}

void initMovementModes() {
  // Initialize random seed
  randomSeed(analogRead(0));
  
  // Select initial mode and duration
  currentModeIndex = 0; // Start with the first mode (Spin)
  currentDurationIndex = 0; // Start with the first duration (5 seconds)
  currentMode = &movementModes[currentModeIndex];
  modeStartTime = millis();
  inRestPeriod = false;
  
  Serial.println("Movement modes initialized");
  Serial.print("Initial mode: ");
  Serial.print(currentMode->name);
  Serial.print(", Duration: ");
  Serial.print(DURATION_OPTIONS[currentDurationIndex]);
  Serial.println(" seconds");
}

void selectNextMode() {
  if (inRestPeriod) {
    // Coming out of rest, select next active mode
    inRestPeriod = false;
    currentModeIndex = (currentModeIndex + 1) % (NUM_MODES - 1); // Skip the REST mode
    currentMode = &movementModes[currentModeIndex];
    
    // Select next duration
    currentDurationIndex = (currentDurationIndex + 1) % NUM_DURATION_OPTIONS;
  } else {
    // Entering rest period
    inRestPeriod = true;
    currentModeIndex = MODE_REST;
    currentMode = &movementModes[currentModeIndex];
    
    // Random rest duration
    restDuration = getRandomRestDuration();
  }
  
  // Reset timers
  modeStartTime = millis();
  lastMovementUpdate = millis();
  lastAuxPinUpdate = millis();
  
  Serial.println("\n--- Mode Change ---");
  Serial.print("New mode: ");
  Serial.print(currentMode->name);
  
  if (inRestPeriod) {
    Serial.print(", Rest Duration: ");
    Serial.print(restDuration);
    Serial.println(" seconds");
  } else {
    Serial.print(", Duration: ");
    Serial.print(DURATION_OPTIONS[currentDurationIndex]);
    Serial.println(" seconds");
  }
  
  Serial.print("Movement interval: ");
  Serial.print(currentMode->movementInterval);
  Serial.print("ms, Aux pin interval: ");
  Serial.print(currentMode->auxPinInterval);
  Serial.println("ms");
}

void updateCurrentMode() {
  unsigned long currentTime = millis();
  
  // Check if it's time to change mode
  if (inRestPeriod) {
    if (currentTime - modeStartTime >= (restDuration * 1000)) {
      Serial.print("Rest period ended after ");
      Serial.print(restDuration);
      Serial.println(" seconds");
      selectNextMode();
      return;
    }
  } else if (currentTime - modeStartTime >= (DURATION_OPTIONS[currentDurationIndex] * 1000)) {
    Serial.print("Mode timeout reached after ");
    Serial.print((currentTime - modeStartTime) / 1000);
    Serial.println(" seconds");
    selectNextMode();
    return;
  }
  
  // Update movement if needed
  if (currentTime - lastMovementUpdate >= currentMode->movementInterval) {
    lastMovementUpdate = currentTime;
    Serial.print("Updating movement pattern: ");
    Serial.println(currentMode->name);
    currentMode->movementFunction();
  }
  
  // Update aux pin if needed
  if (currentTime - lastAuxPinUpdate >= currentMode->auxPinInterval) {
    lastAuxPinUpdate = currentTime;
    Serial.print("Updating aux pin behavior for mode: ");
    Serial.println(currentMode->name);
    currentMode->auxPinFunction();
  }
}

MovementMode* getCurrentMode() {
  return currentMode;
}

int getCurrentModeDuration() {
  if (inRestPeriod) {
    return restDuration;
  }
  return DURATION_OPTIONS[currentDurationIndex];
}

unsigned long getModeStartTime() {
  return modeStartTime;
}

// Movement pattern implementations
void spinPattern() {
  // Alternate between left and right turns
  static bool turnLeft = true;
  Serial.print("Spin pattern: turning ");
  Serial.println(turnLeft ? "LEFT" : "RIGHT");
  setDirection(turnLeft ? TURN_LEFT : TURN_RIGHT);
  turnLeft = !turnLeft;
}

void wanderPattern() {
  // Randomly choose a direction
  int direction = random(4);  // 0-3
  const char* directionStr;
  switch (direction) {
    case 0: setDirection(FORWARD); directionStr = "FORWARD"; break;
    case 1: setDirection(BACKWARD); directionStr = "BACKWARD"; break;
    case 2: setDirection(TURN_LEFT); directionStr = "TURN_LEFT"; break;
    case 3: setDirection(TURN_RIGHT); directionStr = "TURN_RIGHT"; break;
  }
  Serial.print("Wander pattern: random direction = ");
  Serial.println(directionStr);
}

void pulsePattern() {
  // Alternate between forward and backward
  static bool forward = true;
  Serial.print("Pulse pattern: ");
  Serial.println(forward ? "FORWARD" : "BACKWARD");
  setDirection(forward ? FORWARD : BACKWARD);
  forward = !forward;
}

void circlePattern() {
  // Create a circle by turning one motor faster than the other
  static int phase = 0;
  Serial.print("Circle pattern: phase = ");
  Serial.println(phase);
  
  // Debug the MOTOR_SPEED_ACTUAL value
  Serial.print("MOTOR_SPEED_ACTUAL = ");
  Serial.println(MOTOR_SPEED_ACTUAL);
  
  try {
    switch (phase) {
      case 0: 
        Serial.println("Setting Motor A to full speed, Motor B to half speed");
        ledcWrite(0, MOTOR_SPEED_ACTUAL);  // Motor A full speed
        ledcWrite(1, 0);
        ledcWrite(2, MOTOR_SPEED_ACTUAL/2);  // Motor B half speed
        ledcWrite(3, 0);
        break;
      case 1:
        Serial.println("Setting Motor A to half speed, Motor B to full speed");
        ledcWrite(0, MOTOR_SPEED_ACTUAL/2);  // Motor A half speed
        ledcWrite(1, 0);
        ledcWrite(2, MOTOR_SPEED_ACTUAL);  // Motor B full speed
        ledcWrite(3, 0);
        break;
    }
    phase = (phase + 1) % 2;
  } catch (const std::exception& e) {
    Serial.print("Exception in circlePattern: ");
    Serial.println(e.what());
  } catch (...) {
    Serial.println("Unknown exception in circlePattern");
  }
}

void zigzagPattern() {
  // Alternate between forward and turns
  static int phase = 0;
  const char* phaseStr;
  switch (phase) {
    case 0: setDirection(FORWARD); phaseStr = "FORWARD"; break;
    case 1: setDirection(TURN_LEFT); phaseStr = "TURN_LEFT"; break;
    case 2: setDirection(FORWARD); phaseStr = "FORWARD"; break;
    case 3: setDirection(TURN_RIGHT); phaseStr = "TURN_RIGHT"; break;
  }
  Serial.print("Zigzag pattern: phase = ");
  Serial.print(phase);
  Serial.print(", direction = ");
  Serial.println(phaseStr);
  phase = (phase + 1) % 4;
}

void stopPattern() {
  Serial.println("Stop pattern: motors stopped");
  setDirection(STOP);
}

// Aux pin behavior implementations
void auxPinBlink() {
  Serial.println("Aux pin: BLINK");
  // Just toggle the pin - non-blocking
  static bool auxState = false;
  auxState = !auxState;
  digitalWrite(AUX_PIN, auxState);
}

void auxPinPulse() {
  // Create a pulsing effect (non-blocking)
  Serial.println("Aux pin: PULSE");
  static int pulsePhase = 0;
  static bool auxState = false;
  
  if (pulsePhase % 2 == 0) {
    auxState = !auxState;
    digitalWrite(AUX_PIN, auxState);
  }
  
  pulsePhase = (pulsePhase + 1) % 10;
}

void auxPinWave() {
  // Create a wave effect (non-blocking)
  Serial.println("Aux pin: WAVE");
  static int wavePhase = 0;
  
  if (wavePhase < 3) {
    digitalWrite(AUX_PIN, HIGH);
  } else {
    digitalWrite(AUX_PIN, LOW);
  }
  
  wavePhase = (wavePhase + 1) % 6;
}

void auxPinRandom() {
  // Random on/off pattern (non-blocking)
  bool shouldTurnOn = random(2) == 0;
  Serial.print("Aux pin: RANDOM (");
  Serial.print(shouldTurnOn ? "ON" : "OFF");
  Serial.println(")");
  
  digitalWrite(AUX_PIN, shouldTurnOn);
}

void auxPinOff() {
  Serial.println("Aux pin: OFF");
  digitalWrite(AUX_PIN, LOW);
}

void restPattern() {
  // Rest pattern - just stop motors
  Serial.println("Rest pattern: motors stopped");
  setDirection(STOP);
} 