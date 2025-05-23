#ifndef MOVEMENT_MODES_H
#define MOVEMENT_MODES_H

#include <Arduino.h>
#include "motor_control.h"

// Mode duration options (in seconds)
const int DURATION_OPTIONS[] = {5, 10, 15, 20, 25, 30};
const int NUM_DURATION_OPTIONS = sizeof(DURATION_OPTIONS) / sizeof(DURATION_OPTIONS[0]);

// Rest period durations (in seconds)
const int MIN_REST_DURATION = 5;
const int MAX_REST_DURATION = 15;

// Movement mode structure
struct MovementMode {
  const char* name;                    // Name of the mode
  void (*movementFunction)();          // Function pointer to movement pattern
  void (*auxPinFunction)();            // Function pointer to aux pin behavior
  unsigned long movementInterval;      // How often to update movement (ms)
  unsigned long auxPinInterval;        // How often to update aux pin (ms)
  unsigned long auxPinDuration;        // How long aux pin stays on (ms)
};

// Movement mode IDs
enum ModeID {
  MODE_SPIN,           // Spin in place
  MODE_WANDER,         // Random wandering
  MODE_PULSE,          // Forward/backward pulsing
  MODE_CIRCLE,         // Circle pattern
  MODE_ZIGZAG,         // Zigzag pattern
  MODE_STOP,           // Stop and wait
  MODE_REST,           // Rest period between active modes
  NUM_MODES            // Number of available modes
};

// Function declarations
void initMovementModes();
void selectNextMode();
void updateCurrentMode();
MovementMode* getCurrentMode();
int getCurrentModeDuration();
unsigned long getModeStartTime();
int getRandomRestDuration();

// Movement pattern functions
void spinPattern();
void wanderPattern();
void pulsePattern();
void circlePattern();
void zigzagPattern();
void stopPattern();
void restPattern();

// Aux pin behavior functions
void auxPinBlink();
void auxPinPulse();
void auxPinWave();
void auxPinRandom();
void auxPinOff();

#endif // MOVEMENT_MODES_H 