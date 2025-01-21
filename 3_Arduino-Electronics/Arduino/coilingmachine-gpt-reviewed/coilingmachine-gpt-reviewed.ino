// Stepper Weaving Machine - Optimized with Direction Control
#include <Arduino.h>

// Global Flags
bool TECE = false;
bool stepPulseOn = false, stepPulseOff = false;

// Pin Assignments
const int buttonAvance = 2;
const int buttonReturn = 3;
const int buttonLiga = 4;
const int buttonPara = 5;
const int stp = 9;  // Step pin
const int dir = 8;  // Direction pin
const int pot = A0; // Potentiometer for speed control

// Debounce timing
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // in milliseconds

// Button States
int button_Avance_State = LOW;
int button_Return_State = LOW;
int button_Liga_State = LOW;
int button_Para_State = LOW;

// Stepper Motor Speed
int stepDelay = 1000; // Default speed in microseconds
unsigned long stepTimer = 0;

// Function to set motor direction
// Pass `true` for clockwise (CW) and `false` for counterclockwise (CCW)
void setDirection(bool clockwise) {
  digitalWrite(dir, clockwise ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);

  // Configure pins
  pinMode(buttonAvance, INPUT_PULLUP);
  pinMode(buttonReturn, INPUT_PULLUP);
  pinMode(buttonLiga, INPUT_PULLUP);
  pinMode(buttonPara, INPUT_PULLUP);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);

  // Initial setup
  setDirection(false); // Default direction: counterclockwise (CCW)
}

void loop() {
  // Read button states
  int Avance = digitalRead(buttonAvance);
  int Return = digitalRead(buttonReturn);
  int Liga = digitalRead(buttonLiga);
  int Para = digitalRead(buttonPara);

  // Debouncing
  if ((Avance != button_Avance_State) || (Return != button_Return_State) ||
      (Liga != button_Liga_State) || (Para != button_Para_State)) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      button_Avance_State = Avance;
      button_Return_State = Return;
      button_Liga_State = Liga;
      button_Para_State = Para;
      lastDebounceTime = millis();
    }
  }

  // Button Logic
  if (button_Avance_State == LOW) {
    setDirection(false); // Set direction: counterclockwise (CCW)
    stepMotor(20);
  }

  if (button_Return_State == LOW) {
    setDirection(true); // Set direction: clockwise (CW)
    stepMotor(20);
  }

  if (button_Liga_State == LOW) {
    TECE = true;
  }

  if (button_Para_State == LOW) {
    TECE = false;
    digitalWrite(stp, LOW);
  }

  // Continuous Weaving
  if (TECE) {
    stepDelay = map(analogRead(pot), 0, 1023, 2000, 10);
    if ((micros() - stepTimer) > stepDelay) {
      digitalWrite(stp, !digitalRead(stp));
      stepTimer = micros();
    }
  }
}

// Step Motor Function
void stepMotor(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(stp, HIGH);
    delayMicroseconds(500);
    digitalWrite(stp, LOW);
    delayMicroseconds(500);
  }
}
