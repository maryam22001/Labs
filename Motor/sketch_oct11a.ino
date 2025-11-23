/*
 * AVR Basic I/O Interfacing - Stepper Motor Control
 * Name: [Your Name Here]
 * Date: [Current Date]
 */

// Pin Definitions
const int BTN_MODE = 2;
const int BTN_START_STOP = 3;
const int BTN_DIR = 4;
const int BTN_SPEED_UP = 5;
const int BTN_SPEED_DOWN = 6;

const int LED_STATUS = 7;
const int LED_DIR = 8;

// 7-Segment Pins
const int SEG_A = 9;
const int SEG_B = 10;
const int SEG_C = 11;
const int SEG_D = 12;
const int SEG_E = 13;
const int SEG_F = A0;
const int SEG_G = A1;

// Stepper Motor Pins
const int STEPPER_1 = A2;
const int STEPPER_2 = A3;
const int STEPPER_3 = A4;
const int STEPPER_4 = A5;

// Operation Modes
enum Mode {
  MODE_STOP = 0,
  MODE_MANUAL_HALF = 1,
  MODE_MANUAL_FULL = 2,
  MODE_AUTO_HALF = 3,
  MODE_AUTO_FULL = 4,
  MODE_EMERGENCY = 5
};

// Global Variables
volatile Mode currentMode = MODE_STOP;
volatile bool isRunning = false;
volatile bool directionForward = true;
volatile bool emergencyStop = false;

unsigned long lastStepTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long emergencyStartTime = 0;
unsigned long lastDisplayUpdate = 0;

int stepDelay = 500; // ms between steps
int currentStep = 0;

// Stepper Motor Sequences
// Full step sequence (4 steps)
const byte fullStep[4] = {
  B00001001, // Coils 1 & 4 energized
  B00000011, // Coils 1 & 2 energized  
  B00000110, // Coils 2 & 3 energized
  B00001100  // Coils 3 & 4 energized
};

// Half step sequence (8 steps)
const byte halfStep[8] = {
  B00001001, // Coil 1
  B00000011, // Coils 1 & 2
  B00000010, // Coil 2
  B00000110, // Coils 2 & 3
  B00000100, // Coil 3
  B00001100, // Coils 3 & 4
  B00001000, // Coil 4
  B00001001  // Coils 4 & 1
};

// 7-Segment display patterns for numbers 0-4 and E
const byte segPatterns[6] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01111001  // E
};

void setup() {
  // Initialize button pins with pull-up resistors
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_START_STOP, INPUT_PULLUP);
  pinMode(BTN_DIR, INPUT_PULLUP);
  pinMode(BTN_SPEED_UP, INPUT_PULLUP);
  pinMode(BTN_SPEED_DOWN, INPUT_PULLUP);
  
  // Initialize LED pins
  pinMode(LED_STATUS, OUTPUT);
  pinMode(LED_DIR, OUTPUT);
  
  // Initialize 7-segment pins
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  
  // Initialize stepper motor pins
  pinMode(STEPPER_1, OUTPUT);
  pinMode(STEPPER_2, OUTPUT);
  pinMode(STEPPER_3, OUTPUT);
  pinMode(STEPPER_4, OUTPUT);
  
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Stepper Motor Control System Initialized");
  
  // Initial display
  updateDisplay();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check for emergency stop condition (hold START/STOP for 2 seconds)
  checkEmergencyStop();
  
  if (emergencyStop) {
    handleEmergencyMode();
    return;
  }
  
  // Button handling with debouncing
  if (currentTime - lastDebounceTime > 50) {
    handleButtons();
    lastDebounceTime = currentTime;
  }
  
  // Update LEDs based on state
  digitalWrite(LED_STATUS, isRunning);
  digitalWrite(LED_DIR, directionForward);
  
  // Stepper motor control
  if (isRunning) {
    switch(currentMode) {
      case MODE_MANUAL_HALF:
      case MODE_MANUAL_FULL:
        // In manual mode, stepping is handled by button press
        break;
        
      case MODE_AUTO_HALF:
      case MODE_AUTO_FULL:
        if (currentTime - lastStepTime >= stepDelay) {
          performStep();
          lastStepTime = currentTime;
        }
        break;
        
      default:
        break;
    }
  }
  
  // Update display periodically
  if (currentTime - lastDisplayUpdate >= 200) {
    updateDisplay();
    lastDisplayUpdate = currentTime;
  }
}

void checkEmergencyStop() {
  static unsigned long pressStartTime = 0;
  static bool wasPressed = false;
  
  bool isPressed = (digitalRead(BTN_START_STOP) == LOW);
  
  if (isPressed && !wasPressed) {
    // Button just pressed
    pressStartTime = millis();
    wasPressed = true;
  } 
  else if (!isPressed && wasPressed) {
    // Button released
    wasPressed = false;
  }
  
  if (isPressed && (millis() - pressStartTime > 2000)) {
    // Held for more than 2 seconds
    if (!emergencyStop) {
      // Enter emergency stop
      emergencyStop = true;
      isRunning = false;
      emergencyStartTime = millis();
      stopStepper();
      Serial.println("EMERGENCY STOP ACTIVATED");
    } else {
      // Exit emergency stop
      emergencyStop = false;
      currentMode = MODE_STOP;
      Serial.println("EMERGENCY STOP CLEARED");
    }
    // Reset button state
    wasPressed = false;
    delay(500); // Prevent immediate re-trigger
  }
}

void handleEmergencyMode() {
  static unsigned long lastBlinkTime = 0;
  unsigned long currentTime = millis();
  
  // Blink 'E' on display
  if (currentTime - lastBlinkTime >= 500) {
    static bool displayOn = false;
    if (displayOn) {
      displayCharacter(5); // Display 'E'
    } else {
      clearDisplay();
    }
    displayOn = !displayOn;
    lastBlinkTime = currentTime;
  }
  
  // All outputs should be off in emergency
  digitalWrite(LED_STATUS, LOW);
  digitalWrite(LED_DIR, LOW);
  stopStepper();
  
  // Check for emergency clear (handled in checkEmergencyStop)
}

void handleButtons() {
  // Mode button - cycle through modes
  if (digitalRead(BTN_MODE) == LOW) {
    cycleMode();
    delay(300); // Simple debounce
  }
  
  // Start/Stop button (short press)
  if (digitalRead(BTN_START_STOP) == LOW && !emergencyStop) {
    toggleStartStop();
    delay(300); // Simple debounce
  }
  
  // Direction button
  if (digitalRead(BTN_DIR) == LOW) {
    toggleDirection();
    delay(300); // Simple debounce
  }
  
  // Speed control buttons
  if (digitalRead(BTN_SPEED_UP) == LOW) {
    adjustSpeed(true); // Increase speed
    delay(200);
  }
  
  if (digitalRead(BTN_SPEED_DOWN) == LOW) {
    adjustSpeed(false); // Decrease speed
    delay(200);
  }
}

void cycleMode() {
  if (emergencyStop) return;
  
  currentMode = (Mode)((currentMode + 1) % 5); // Cycle through 0-4
  
  Serial.print("Mode changed to: ");
  Serial.println(currentMode);
  
  // If switching to manual mode and running, perform one step
  if (isRunning && (currentMode == MODE_MANUAL_HALF || currentMode == MODE_MANUAL_FULL)) {
    performStep();
  }
}

void toggleStartStop() {
  if (emergencyStop) return;
  
  isRunning = !isRunning;
  
  Serial.print("Motor ");
  Serial.println(isRunning ? "STARTED" : "STOPPED");
  
  if (!isRunning) {
    stopStepper();
  }
}

void toggleDirection() {
  if (emergencyStop) return;
  
  directionForward = !directionForward;
  
  Serial.print("Direction: ");
  Serial.println(directionForward ? "FORWARD" : "REVERSE");
}

void adjustSpeed(bool increase) {
  if (emergencyStop) return;
  
  if (increase) {
    stepDelay = max(50, stepDelay - 50); // Minimum 50ms delay
  } else {
    stepDelay = min(2000, stepDelay + 50); // Maximum 2000ms delay
  }
  
  Serial.print("Step delay: ");
  Serial.println(stepDelay);
}

void performStep() {
  if (emergencyStop) return;
  
  switch(currentMode) {
    case MODE_MANUAL_HALF:
    case MODE_AUTO_HALF:
      performHalfStep();
      break;
      
    case MODE_MANUAL_FULL:
    case MODE_AUTO_FULL:
      performFullStep();
      break;
      
    default:
      break;
  }
}

void performFullStep() {
  byte stepPattern;
  
  if (directionForward) {
    stepPattern = fullStep[currentStep];
    currentStep = (currentStep + 1) % 4;
  } else {
    currentStep = (currentStep - 1 + 4) % 4;
    stepPattern = fullStep[currentStep];
  }
  
  setStepperOutputs(stepPattern);
}

void performHalfStep() {
  byte stepPattern;
  
  if (directionForward) {
    stepPattern = halfStep[currentStep];
    currentStep = (currentStep + 1) % 8;
  } else {
    currentStep = (currentStep - 1 + 8) % 8;
    stepPattern = halfStep[currentStep];
  }
  
  setStepperOutputs(stepPattern);
}

void setStepperOutputs(byte pattern) {
  digitalWrite(STEPPER_1, pattern & 0x01);
  digitalWrite(STEPPER_2, pattern & 0x02);
  digitalWrite(STEPPER_3, pattern & 0x04);
  digitalWrite(STEPPER_4, pattern & 0x08);
}

void stopStepper() {
  digitalWrite(STEPPER_1, LOW);
  digitalWrite(STEPPER_2, LOW);
  digitalWrite(STEPPER_3, LOW);
  digitalWrite(STEPPER_4, LOW);
}

void updateDisplay() {
  if (emergencyStop) {
    displayCharacter(5); // Display 'E'
  } else {
    displayCharacter(currentMode);
  }
}

void displayCharacter(int number) {
  if (number < 0 || number > 5) return;
  
  byte pattern = segPatterns[number];
  
  digitalWrite(SEG_A, pattern & 0x01);
  digitalWrite(SEG_B, pattern & 0x02);
  digitalWrite(SEG_C, pattern & 0x04);
  digitalWrite(SEG_D, pattern & 0x08);
  digitalWrite(SEG_E, pattern & 0x10);
  digitalWrite(SEG_F, pattern & 0x20);
  digitalWrite(SEG_G, pattern & 0x40);
}

void clearDisplay() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}