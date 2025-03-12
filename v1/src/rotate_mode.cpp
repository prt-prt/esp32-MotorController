#include "rotate_mode.h"
#include "motor_control.h"

// Rotate mode - slow rotation around its own axis
void runRotateMode() {
  // Slow rotation speed (30% of max speed)
  int rotationSpeed = MAX_SPEED * 0.3;
  
  // Choose a random rotation direction, but change it less frequently
  static int rotationDirection = 0;
  static unsigned long lastDirectionChange = 0;
  
  if (millis() - lastDirectionChange > 10000 || lastDirectionChange == 0) { // Change every 10 seconds
    lastDirectionChange = millis();
    rotationDirection = random(2); // 0 or 1
    
    if (rotationDirection == 0) {
      Serial.print("Rotate: Turning left at speed ");
      Serial.println(rotationSpeed);
    } else {
      Serial.print("Rotate: Turning right at speed ");
      Serial.println(rotationSpeed);
    }
  }
  
  // Perform the rotation
  if (rotationDirection == 0) {
    turnLeft(rotationSpeed);
  } else {
    turnRight(rotationSpeed);
  }
  
  // Small delay to prevent too frequent serial prints
  delay(1000);
} 