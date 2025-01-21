//    Maquina dse Tecelagem Jasmim
//    1 botao para tecer
//    1 botao para parar de tecer
//    1 botao para girar passo a passo para frente
//    1 botao para girar passoa a passo para trás
//    1 potenciometro para regular a velocidade
//    1 chave para ligar e desligar a tecelagem

// Versao 01 --> Versao inicial

//#include <Stepper.h>
bool TECE = 0;

//const int buttonAvance = 2;     // the number of the pushbutton pin
//const int buttonReturn = 3;     // the number of the pushbutton pin
//const int buttonLiga = 4;     // the number of the pushbutton pin
//const int buttonPara = 5;     // the number of the pushbutton pin
const int buttonLiga = 2;     // the number of the pushbutton pin
const int buttonPara = 3;     // the number of the pushbutton pin
const int buttonAvance = 4;     // the number of the pushbutton pin
const int buttonReturn = 5;     // the number of the pushbutton pin
int stp = 9;  //connect pin 6 to step
int dir = 8;  // connect pin 7 to direcao
int velocidade = 0;
#define pot A0


// variables will change:
int button_Avance_State = 0;         // variable for Avance the pushbutton status
int button_Return_State = 0;         // variable for Avance the pushbutton status
int button_Liga_State = 0;         // variable for Avance the pushbutton status
int button_Para_State = 0;         // variable for Avance the pushbutton status

int stepCount = 0;         // number of steps the motor has taken

int Soma = 0;

int Memoria_Avance = LOW;   // the previous Avance from the input pin
int Memoria_Return = LOW;   // the previous Avance from the input pin
int Memoria_Liga = LOW;   // the previous Avance from the input pin
int Memoria_Para = LOW;   // the previous Avance from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(buttonAvance, INPUT);
  pinMode(buttonReturn, INPUT);
  pinMode(buttonLiga, INPUT);
  pinMode(buttonPara, INPUT);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, HIGH);
  //digitalWrite(dir2, LOW);
  delay(100);
}

void loop() {
  // read the state of the switch into a local variable:
  int Avance = digitalRead(buttonAvance);
  int Return = digitalRead(buttonReturn);
  int Liga = digitalRead(buttonLiga);
  int Para = digitalRead(buttonPara);
  //Serial.println(Return);

  // If the switch changed, due to noise or pressing:
  if ((Avance != Memoria_Avance) or (Return != Memoria_Return) or (Liga != Memoria_Liga) or (Para != Memoria_Para)){
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
    if (Avance != button_Avance_State) {
      button_Avance_State = Avance;
      if (button_Avance_State == HIGH) {
          Soma++;
          digitalWrite(dir, HIGH);
          //digitalWrite(dir2, LOW);
         /* digitalWrite(stp, HIGH); 
          delayMicroseconds(1);
          digitalWrite(stp, LOW); 
          delayMicroseconds(1); 
          Serial.println(Soma);     
          Serial.print("steps:");
          Serial.println(stepCount);
          stepCount++;
          Serial.println(Soma);*/
         analogWrite(stp, 10);
          delay(50);
          analogWrite(stp, 0);
        }
      }  
          if (Return != button_Return_State) {
      button_Return_State = Return;
      if (button_Return_State == HIGH) {
          Soma++;
          digitalWrite(dir, LOW);
          //digitalWrite(dir2, HIGH);
          analogWrite(stp, 10);
          delay(50);
          analogWrite(stp, 0);
          /*digitalWrite(stp, HIGH); 
          delayMicroseconds(1);
          digitalWrite(stp, LOW); 
          delayMicroseconds(1); 
          Serial.println(Soma);     
          Serial.print("steps:");
          Serial.println(stepCount);
          stepCount++;
          Serial.println(Soma);*/
        }
      } 
      if (Para != button_Para_State) {
       button_Para_State = Para;
       if (button_Para_State == HIGH) {TECE = 0;   analogWrite(stp, 0);}

       
      }
      if (Liga != button_Liga_State) {
       button_Liga_State = Liga;
       if (button_Liga_State == HIGH) TECE = 1;
       digitalWrite(dir, HIGH);
       //digitalWrite(dir2, LOW); 
      }
    }

  // save the Avance. Next time through the loop, it'll be the Memoria_Avance:
  Memoria_Avance = Avance;  
  Memoria_Return = Return;
  Memoria_Liga = Liga;
  Memoria_Para = Para;
  if (TECE == 1){
  //Soma++;
  //velocidade = analogRead(pot);
  velocidade = map((analogRead(pot)), 0,1023, 100, 10);
  //digitalWrite(stp, HIGH); 
  analogWrite(stp, velocidade);
  /*delayMicroseconds(velocidade);
  digitalWrite(stp, LOW); 
  delayMicroseconds(velocidade); */
  Serial.println("velocidade");
  Serial.println(velocidade);
   
  }
}
