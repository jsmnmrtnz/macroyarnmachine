//    Macro Yarn Machine, designed by Jasmin Martinez, 2024
//    Paludi Coiling Machine, designed by Jasmin Martinez, 2025
//    Code by Julio Augusto Martinez, 2024

// To change motor direction, update the "motorDirection" variable to HIGH or LOW.
// Use HIGH for one direction and LOW for the reverse direction.

#include <Arduino.h>

// Variables to control the motor
const int motorStepPin = 9;  // Connect to the STEP pin
const int motorDirPin = 8;   // Connect to the DIR pin

// Define the default motor direction
// Change this value to set the initial direction: HIGH or LOW
// For the Coiling Machine, HIGH is counterclockwise.

int motorDirection = HIGH; 

bool TECE = 0;
int x = 0;

// Pin assignments for buttons
const int buttonAvance = 2;     
const int buttonReturn = 3;    
const int buttonLiga = 4;      
const int buttonPara = 5;      
const int potPin = A0;         

// LED pins
const int Led_Tece = 10;       
const int Led_Para = 11;       

// Variables to handle button states
bool chaveon = 0, chaveoff = 0, chaveEnd = 0;
int button_Avance_State = 0, button_Return_State = 0, button_Liga_State = 0, button_Para_State = 0;
int Memoria_Avance = LOW, Memoria_Return = LOW, Memoria_Liga = LOW, Memoria_Para = LOW;

// Debounce settings
unsigned long lastDebounceTime = 0;  
const unsigned long debounceDelay = 50; 

void setup() {
  Serial.begin(9600);

  // Configure pins
  pinMode(buttonAvance, INPUT);
  pinMode(buttonReturn, INPUT);
  pinMode(buttonLiga, INPUT);
  pinMode(buttonPara, INPUT);
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(Led_Tece, OUTPUT);
  pinMode(Led_Para, OUTPUT);

  // Set initial motor direction
  digitalWrite(motorDirPin, motorDirection); 
  delay(100);
}

void loop() {
  // Read button states
  int Avance = digitalRead(buttonAvance);
  int Return = digitalRead(buttonReturn);
  int Liga = digitalRead(buttonLiga);

  // Debounce buttons
  if ((Avance != Memoria_Avance) || (Return != Memoria_Return) || (Liga != Memoria_Liga)) {
    lastDebounceTime = micros();
  }

  if ((micros() - lastDebounceTime) > debounceDelay) {
    if (Avance != button_Avance_State) {
      button_Avance_State = Avance;
      if (button_Avance_State == HIGH) {
        motorDirection = LOW;  // Set direction for avance
        digitalWrite(motorDirPin, motorDirection);
        stepMotor(20);
      }
    }

    if (Return != button_Return_State) {
      button_Return_State = Return;
      if (button_Return_State == HIGH) {
        motorDirection = HIGH;  // Set direction for return
        digitalWrite(motorDirPin, motorDirection);
        stepMotor(20);
      }
    }

    if (Liga != button_Liga_State) {
      button_Liga_State = Liga;
      if (button_Liga_State == HIGH) TECE = 1;
    }
  }

  // Save button states
  Memoria_Avance = Avance;
  Memoria_Return = Return;
  Memoria_Liga = Liga;

  // Handle motor operation
  manageMotor();
}

void stepMotor(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(motorStepPin, HIGH);
    delay(1);
    digitalWrite(motorStepPin, LOW);
    delay(1);
  }
}

void manageMotor() {
  if (TECE == 1) {
    digitalWrite(Led_Tece, HIGH);
    digitalWrite(Led_Para, LOW);

    while (TECE == 1) {
      int sp = map(analogRead(potPin), 0, 1023, 1000, 0);
      digitalWrite(motorStepPin, HIGH);
      delayMicroseconds(sp + 275);
      digitalWrite(motorStepPin, LOW);
      delayMicroseconds(sp + 275);

      // Handle stop button
      int Para = digitalRead(buttonPara);
      if ((Para != Memoria_Para) && (micros() - lastDebounceTime > debounceDelay)) {
        button_Para_State = Para;
        if (button_Para_State == HIGH) {
          TECE = 0;
          digitalWrite(motorStepPin, LOW);
        }
      }
      Memoria_Para = Para;
    }
  } else {
    digitalWrite(Led_Tece, LOW);
    digitalWrite(Led_Para, HIGH);
  }
}
