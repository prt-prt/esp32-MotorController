#include "wander_mode.h"
#include "motor_control.h"

// Wander mode - randomized organic movement with varying speeds
void runWanderMode() {
  // Select a movement pattern (more complex with 8 options)
  int movementType = random(8); // 0-7
  
  // Base speed and speed difference for curves
  int baseSpeed = random(MIN_SPEED + 50, MAX_SPEED);
  int speedDiff = random(20, 100);  // Difference between wheels for curves
  int duration = random(500, 3000); // Movement duration
  
  // Variables for differential drive
  int leftSpeed = 0;
  int rightSpeed = 0;
  
  switch (movementType) {
    case 0: // Straight forward
      Serial.print("Wander: Forward at speed ");
      Serial.println(baseSpeed);
      moveForward(baseSpeed);
      break;
      
    case 1: // Gentle left curve
      leftSpeed = baseSpeed - speedDiff;
      rightSpeed = baseSpeed;
      Serial.print("Wander: Gentle left curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 2: // Gentle right curve
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed - speedDiff;
      Serial.print("Wander: Gentle right curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 3: // Sharp left curve
      leftSpeed = baseSpeed - speedDiff*1.5;
      rightSpeed = baseSpeed;
      Serial.print("Wander: Sharp left curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 4: // Sharp right curve
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed - speedDiff*1.5;
      Serial.print("Wander: Sharp right curve (L:");
      Serial.print(leftSpeed);
      Serial.print(", R:");
      Serial.print(rightSpeed);
      Serial.println(")");
      moveDifferential(leftSpeed, rightSpeed);
      break;
      
    case 5: // Spin turn - traditional on-spot turn
      if (random(2) == 0) { // Randomly choose direction
        Serial.print("Wander: Spin left at speed ");
        Serial.println(baseSpeed);
        turnLeft(baseSpeed);
      } else {
        Serial.print("Wander: Spin right at speed ");
        Serial.println(baseSpeed);
        turnRight(baseSpeed);
      }
      break;
      
    case 6: // Backward move
      Serial.print("Wander: Backward at speed ");
      Serial.println(baseSpeed);
      moveBackward(baseSpeed);
      break;
      
    case 7: // Stop for a moment
      Serial.println("Wander: Stopping for a moment");
      stopMotors();
      duration = random(1000, 5000); // Stop for 1-5 seconds
      break;
  }
  
  delay(duration);
} 