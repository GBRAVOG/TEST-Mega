#include <Arduino.h>

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_STEP_INT          2    // INT 0 Vector 2
#define X_LINE_START       27    // Start sensor X
#define X_LINE_END         35    // End sensor   X  
#define X_END_LED          43    // END X Line LED

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_STEP_INT          3    // INT 1 Vector 3
#define Y_LINE_START       29
#define Y_LINE_END         37
#define Y_END_LED          45    // END Y Line LED

// #define Z_STEP_PIN         46
// #define Z_DIR_PIN          48
// #define Z_ENABLE_PIN       62


#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24
#define E_STEP_INT         19     // INT 4 Vector 19
#define E_LINE_START       31
#define E_LINE_END         39
#define E_END_LED          47    // END E Line LED

#define Q_STEP_PIN         36            
#define Q_DIR_PIN          34
#define Q_ENABLE_PIN       30
#define Q_STEP_INT         20     // INT 3 Vector 20
#define Q_LINE_START       33
#define Q_LINE_END         41
#define Q_END_LED          32    // END Q Line LED

// #define INT2        21       // INT 2 Vector 21
// #define INT5        18       // INT 5 Vector 18

// #define SDPOWER            -1
//#define SDSS               53

#define LED_PIN            13
#define Start_Button       23    // Ex Y_MIN_PIN (Y-) 14
#define Pause_Button       25    // Ex Y_MAX_PIN (Y+) 15

#define SPEED_HIGH          2
#define SPEED_LOW          25
#define STEPS             100     // steps per interup event
#define INT_DEBOUNCE      200     // Delay in ms for Interup debounce

#define FAN_PIN             9
// #define PS_ON_PIN          12

#define HEATER_0_PIN       10	  //PWM PIN!
#define HEATER_1_PIN        8	  //PWM PIN!

// #define TEMP_0_PIN         13	// Analoge numering
// #define TEMP_1_PIN         14	// Analoge numering

volatile int x_count = 0;   // directs the compiler to load the variable from RAM 
volatile int y_count = 0;   // and not from a storage register
volatile int e_count = 0;   // variable stored in registers can be inaccurate 
volatile int q_count = 0;

// volatile unsigned long x_count = 0;   // directs the compiler to load the variable from RAM 
// volatile unsigned long y_count = 0;   // and not from a storage register
// volatile unsigned long e_count = 0;   // variable stored in registers can be inaccurate 
// volatile unsigned long q_count = 0;

// Variable used for millis debounce
unsigned long StartOfLastDebounce = 0;  // holds the last time the Start switch was pressed
unsigned long PauseOfLastDebounce = 0;  // holds the last time the Pause switch was pressed
unsigned long DelayofDebounce = 200;  // amount of time that needs to be experied between presses

volatile long x_debounce_time =0;
volatile long x_cuttent_time =0;
volatile long y_debounce_time =0;
volatile long y_cuttent_time =0;
volatile long q_debounce_time =0;
volatile long q_cuttent_time =0;
volatile long e_debounce_time =0;
volatile long e_cuttent_time =0;

int x_delay = SPEED_LOW;               // Delay between step pulses
int Start_Button_State = 0;
int Pause_Button_State = 0;
boolean IntFlag = false;               // Enable disable Interup functions
boolean RunFlag = false;               // true game runing
boolean EndFlag = false;               // end run sensor
boolean LineToStartFlag = false;       // Mode return to start
int ledState = LOW;                    // HIGH LED ON

// Interup Functions
void x_step_ISR(){                     // Interup Fuction 0 pin D2; Motor X
  x_cuttent_time = millis();
  if(RunFlag && IntFlag && (x_cuttent_time - x_debounce_time > 100)) {
    x_count =x_count + STEPS;                   //x_count Increment; 
  }
  x_debounce_time = x_cuttent_time;
}

void y_step_ISR(){              // Interup Fuction 1 pin D3; Motor Y
  y_cuttent_time = millis();
  if (RunFlag && IntFlag && (y_cuttent_time - y_debounce_time > INT_DEBOUNCE)) {
    y_count =y_count + STEPS;
  }
  y_debounce_time = y_cuttent_time;
}

void e_step_ISR(){              // Interup Fuction 4 PIN 19; Motor E
  e_cuttent_time = millis();
  if (RunFlag && IntFlag && (e_cuttent_time - e_debounce_time > INT_DEBOUNCE)) {  
    e_count =e_count + STEPS;
  }
  e_debounce_time = e_cuttent_time;
}

void q_step_ISR(){             // Interup Fuction 3 pin D20; Motor Q
  q_cuttent_time = millis();
  if (RunFlag && IntFlag && (q_cuttent_time - q_debounce_time > INT_DEBOUNCE)) {
    q_count = q_count + STEPS;
  }
  q_debounce_time = q_cuttent_time;
}

// Services Functions
void set_counters(int c){
  x_count = c;
  y_count = c;
  e_count = c;
  q_count = c;
}

// Setup Initial Configuration
void setup() {
pinMode(FAN_PIN, OUTPUT);
// pinMode(HEATER_0_PIN, OUTPUT);
// pinMode(HEATER_1_PIN, OUTPUT);

pinMode(LED_PIN, OUTPUT);
pinMode(Start_Button, INPUT_PULLUP);
pinMode(Pause_Button, INPUT_PULLUP);

pinMode(X_STEP_PIN, OUTPUT);
pinMode(X_DIR_PIN, OUTPUT);
pinMode(X_ENABLE_PIN, OUTPUT);
pinMode(X_STEP_INT, INPUT_PULLUP);
pinMode(X_LINE_START, INPUT_PULLUP);
pinMode(X_LINE_END, INPUT_PULLUP);
pinMode(X_END_LED, OUTPUT);

pinMode(Y_STEP_PIN, OUTPUT);
pinMode(Y_DIR_PIN, OUTPUT);
pinMode(Y_ENABLE_PIN, OUTPUT);
pinMode(Y_STEP_INT, INPUT_PULLUP);
pinMode(Y_LINE_START, INPUT_PULLUP);
pinMode(Y_LINE_END, INPUT_PULLUP);
pinMode(Y_END_LED, OUTPUT);

//pinMode(Z_STEP_PIN, OUTPUT);
//pinMode(Z_DIR_PIN, OUTPUT);
//pinMode(Z_ENABLE_PIN, OUTPUT);

pinMode(E_STEP_PIN, OUTPUT);
pinMode(E_DIR_PIN, OUTPUT);
pinMode(E_ENABLE_PIN, OUTPUT);
pinMode(E_STEP_INT, INPUT_PULLUP);
pinMode(E_LINE_START, INPUT_PULLUP);
pinMode(E_LINE_END, INPUT_PULLUP);
pinMode(E_END_LED, OUTPUT);

pinMode(Q_STEP_PIN, OUTPUT);
pinMode(Q_DIR_PIN, OUTPUT);
pinMode(Q_ENABLE_PIN, OUTPUT);
pinMode(Q_STEP_INT, INPUT_PULLUP);
pinMode(Q_LINE_START, INPUT_PULLUP);
pinMode(Q_LINE_END, INPUT_PULLUP);
pinMode(Q_END_LED, OUTPUT);

digitalWrite(X_DIR_PIN, HIGH); // sentido del reloj
digitalWrite(Y_DIR_PIN, HIGH); // sentido del reloj
digitalWrite(E_DIR_PIN, HIGH); // sentido del reloj
digitalWrite(Q_DIR_PIN, HIGH); // sentido del reloj

digitalWrite(X_END_LED, HIGH);     // Off LED END X
digitalWrite(Y_END_LED, HIGH);     // Off LED END Y
digitalWrite(E_END_LED, HIGH);     // Off LED END E
digitalWrite(Q_END_LED, HIGH);     // Off LED END Q

digitalWrite(X_ENABLE_PIN, HIGH);  // Disable Motor X
digitalWrite(Y_ENABLE_PIN, HIGH);  // Disable Motor Y
digitalWrite(E_ENABLE_PIN, HIGH);  // Disable Motor E
digitalWrite(Q_ENABLE_PIN, HIGH);  // Disable Motor Q

// x_count = 82;  4.898 steps para 3m
attachInterrupt(digitalPinToInterrupt(X_STEP_INT), x_step_ISR, FALLING);
attachInterrupt(digitalPinToInterrupt(Y_STEP_INT), y_step_ISR, FALLING);
attachInterrupt(digitalPinToInterrupt(E_STEP_INT), e_step_ISR, FALLING);
attachInterrupt(digitalPinToInterrupt(Q_STEP_INT), q_step_ISR, FALLING);


Serial.begin(115200);     // initialize serial communication at 115200 bits per second:
delay(50);
Serial.println("INIT OK");

x_delay = SPEED_LOW;                       // Low speed
set_counters(0);
RunFlag = false;
LineToStartFlag = false;
ledState = LOW;
digitalWrite(LED_PIN, ledState);   // set initial LED state
digitalWrite(FAN_PIN, LOW);        // Fan Led off

}

// Infinite Loop
void loop() {
if(RunFlag){  
if(x_count > 0){
  digitalWrite(X_ENABLE_PIN, LOW); // Enable Motor X
  digitalWrite(X_STEP_PIN, LOW);
  x_count--;
  // ledState = !ledState;
  //Serial.println(x_count);
}
if(y_count > 0){
  digitalWrite(Y_ENABLE_PIN, LOW);  // Enable Motor Y
  digitalWrite(Y_STEP_PIN, LOW);
  y_count--;
}
if (e_count > 0){
  digitalWrite(E_ENABLE_PIN, LOW);  // Enable Motor E
  digitalWrite(E_STEP_PIN, LOW);
  //Serial.println(e_count);
  e_count--;
}
if (q_count > 0){
  digitalWrite(Q_ENABLE_PIN, LOW);  // Enable Motor Q
  digitalWrite(Q_STEP_PIN, LOW);
  q_count--;
}

delayMicroseconds(180);

digitalWrite(X_STEP_PIN, HIGH);
digitalWrite(Y_STEP_PIN, HIGH);
digitalWrite(E_STEP_PIN, HIGH);
digitalWrite(Q_STEP_PIN, HIGH);  

delay(x_delay);

digitalWrite(X_ENABLE_PIN, HIGH);  // Disable Motor X
digitalWrite(Y_ENABLE_PIN, HIGH);  // Disable Motor Y
digitalWrite(E_ENABLE_PIN, HIGH);  // Disable Motor E
digitalWrite(Q_ENABLE_PIN, HIGH);  // Disable Motor Q
}

// Start Button Switch - Debounce using a 'millis' timer
if (digitalRead(Start_Button) == LOW && Start_Button_State == 0){
      
  // check if enough time has passed to consider it a switch press
  if ((millis() - StartOfLastDebounce) > DelayofDebounce){
    Start_Button_State = 1;
    IntFlag = true;
    RunFlag = true;
    EndFlag = false;
    ledState = HIGH;
    StartOfLastDebounce = millis();

    // digitalWrite(X_DIR_PIN, HIGH); // sentido del reloj
    // digitalWrite(Y_DIR_PIN, HIGH); // sentido del reloj
    // digitalWrite(E_DIR_PIN, HIGH); // sentido del reloj
    // digitalWrite(Q_DIR_PIN, HIGH); // sentido del reloj
    x_delay = SPEED_LOW;                       // Low speed
  }
} 
else {
    if (Start_Button_State == 1 && digitalRead(Start_Button) == HIGH){
      Start_Button_State = 0;
    }
}

// PAUSE Button Switch - Debounce using a 'millis' timer
if (digitalRead(Pause_Button) == LOW && Pause_Button_State == 0){
      
  // check if enough time has passed to consider it a switch press
  if ((millis() - PauseOfLastDebounce) > DelayofDebounce) {
    Pause_Button_State = 1;
    RunFlag = false;
    ledState = LOW;
    PauseOfLastDebounce = millis();
  }
} 
else {
    if (Pause_Button_State == 1 && digitalRead(Pause_Button) == HIGH){
      Pause_Button_State = 0;
    }
} 

// Return to Line Start, two buttons
if (digitalRead(Start_Button) == LOW && Start_Button_State == 1 && digitalRead(Pause_Button) == LOW && Pause_Button_State == 1){
  digitalWrite(X_DIR_PIN, LOW);  // Contra sentido del reloj
  digitalWrite(Y_DIR_PIN, LOW);  // Contra sentido del reloj
  digitalWrite(E_DIR_PIN, LOW);  // Contra sentido del reloj
  digitalWrite(Q_DIR_PIN, LOW);  // Contra sentido del reloj
  x_delay = SPEED_HIGH;                       // High speed
  set_counters (5000);
  IntFlag = false;
  RunFlag = true;
  ledState = HIGH;
  LineToStartFlag = true;
}

// Line to Start detect sensor
if(!(digitalRead(X_LINE_START)) && LineToStartFlag){
  x_delay = SPEED_LOW;                       // Low speed
  set_counters(0);
  RunFlag = false;
  digitalWrite(X_DIR_PIN, HIGH); // sentido del reloj
  digitalWrite(Y_DIR_PIN, HIGH); // sentido del reloj
  digitalWrite(E_DIR_PIN, HIGH); // sentido del reloj
  digitalWrite(Q_DIR_PIN, HIGH); // sentido del reloj
  LineToStartFlag = false;
  ledState = LOW;
  digitalWrite(X_END_LED, HIGH);     // Off LED END X
  digitalWrite(Y_END_LED, HIGH);     // Off LED END Y
  digitalWrite(E_END_LED, HIGH);     // Off LED END E
  digitalWrite(Q_END_LED, HIGH);     // Off LED END Q

  digitalWrite(FAN_PIN, LOW);        // Fan Led off
}

// Line to End detect sensor X
if(!(digitalRead(X_LINE_END)) && !EndFlag){
  set_counters(0);
  RunFlag = false;
  ledState = LOW;
  EndFlag = true;
  digitalWrite(X_END_LED, LOW);       // On LED END X
  digitalWrite(FAN_PIN, HIGH);        // Fan Led on
}

// Line to End detect sensor Y
if(!(digitalRead(Y_LINE_END)) && !EndFlag){
  set_counters(0);
  RunFlag = false;
  ledState = LOW;
  EndFlag = true;
  digitalWrite(Y_END_LED, LOW);       // On LED END Y
  digitalWrite(FAN_PIN, HIGH);        // Fan Led on
}

// Line to End detect sensor E
if(!(digitalRead(E_LINE_END)) && !EndFlag){
  set_counters(0);
  RunFlag = false;
  ledState = LOW;
  EndFlag = true;
  digitalWrite(E_END_LED, LOW);       // On LED END E
  digitalWrite(FAN_PIN, HIGH);        // Fan Led on
}

// Line to End detect sensor Q
if(!(digitalRead(Q_LINE_END)) && !EndFlag){
  set_counters(0);
  RunFlag = false;
  ledState = LOW;
  EndFlag = true;
  digitalWrite(Q_END_LED, LOW);       // On LED END Q
  digitalWrite(FAN_PIN, HIGH);        // Fan Led on
}

digitalWrite(LED_PIN, ledState);  
}
