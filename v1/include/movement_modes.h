#ifndef MOVEMENT_MODES_H
#define MOVEMENT_MODES_H

#include <Arduino.h>
#include "test_mode.h"
#include "wander_mode.h"
#include "rotate_mode.h"

// Movement modes enumeration
enum MovementMode {
  TEST_MODE,
  WANDER_MODE,
  ROTATE_MODE
};

#endif // MOVEMENT_MODES_H 