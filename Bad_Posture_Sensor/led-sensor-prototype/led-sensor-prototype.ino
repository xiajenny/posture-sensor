//**************************************************************************************
//**************************************************************************************
//                              POSTURE SENSOR PROJECT
//
//                      BY SIENNA WISHART AND JENNY XIA (GROUP 7)
//                            FOR SYDE192L FINAL PROJECT
//
//                 WITH THANKS TO ORION BRUCKMAN AND CHRIS MCCLELLAN
//                
//             ALL CODE HEREIN IS WRITTEN BY SIENNA WISHART AND JENNY XIA
//
//**************************************************************************************
//**************************************************************************************
//                              SENSOR VARIABLE DECLARATION

int backSensor = 4; //input pin for back sensor
int shoulderLSensor = 5; //input pin for left shoulder sensor
int shoulderRSensor = 6; //input pin for right shoulder sensor
int neckSensor = 7; // input pin for neck sensor

int badPosturePin = 2; //input indicates if any bad posture is sensed
                         //(result of 4 OR gate from back, shoulders, and neck sensors)
volatile bool badPosture = 0;

//**************************************************************************************
//                              TIMER VARIABLE DECLARATION

volatile bool timerP1 = false;
volatile bool timerP2 = false;
volatile bool timerP3 = false;

unsigned long int timer1_ovf = 0;

//**************************************************************************************
//                               LED VARIABLE DECLARATION

int ledPinC1 = 8; 
int ledPinC2 = 9;
int ledPinC3 = 10;
int ledPinC4 = 11;
int ledPinR1 = 12;
int ledPinR2 = 13;

int delayTime = 1;

bool ledA = false; //bad posture started
bool ledB = false; //1.5s of bad posture
bool ledC = false; //3s of bad posture
bool ledD = false; //xs of bad posture

bool ledE = false; //bad back posture
bool ledF = false; //bad left shoulder posture
bool ledG = false; //bad right shoulder posture
bool ledH = false; //bad neck posture

//LED wire layout。 Uses 6 pins to control 8 LEDS.

//         c1    c2    c3    c4
//
//  r1      a     b     c     d
//
//  r2      e     f     g     h     

// a, b, c, d, e, f, g are the LEDS


//**************************************************************************************
//**************************************************************************************
//                               SETUP

void setup() {
  Serial.begin(9600); // enable serial monitor
  //TIMERS & INTERRUPTS
  TCCR1A = 0;  //reset  
  TCCR1B = 0;
  TCCR1B = 5;  //set timer1 prescaler to /1024
  TIMSK1 = 0x1;
  
  attachInterrupt(digitalPinToInterrupt(badPosturePin), postureHandler, CHANGE);  

  OCR1A = 23437; //timer1 compare interruptA value: 1.5 seconds and LED flashes
  OCR1B = 46875; //timer1 compare interruptB value: 3 seconds and LED flashes  

  //SENSORS
  pinMode(badPosturePin, INPUT_PULLUP);
  pinMode(backSensor,INPUT);
  pinMode(shoulderLSensor, INPUT);
  pinMode(shoulderRSensor, INPUT);
  pinMode(neckSensor, INPUT);

  //LEDS
  pinMode (ledPinC1, OUTPUT);
  pinMode (ledPinC2, OUTPUT);
  pinMode (ledPinC3, OUTPUT);
  pinMode (ledPinC4, OUTPUT);
  pinMode (ledPinR1, OUTPUT);
  pinMode (ledPinR2, OUTPUT);
    
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, LOW); 
   /* 
   ledA = 0;
   ledB = 0;
   ledC = 1;
   ledD = 1;
   ledE = 1;
   ledF = 1;
   ledG = 0;
   ledH = 0;*/
}

//**************************************************************************************
//**************************************************************************************
//                              MAIN LOOP

void loop() {
    
  if (badPosture) {
   turnOnA();

   if ( timerP1 ) 
    turnOnB();
   if ( timerP2 ) 
    turnOnC();
    
    
   // BACK
   if ( digitalRead(backSensor) ) 
    turnOnE();
   else 
   turnOffLEDS();

   // LEFT SHOULDER
   if ( digitalRead(shoulderLSensor) ) 
    turnOnF();
   else 
   turnOffLEDS();

   // RIGHT SHOULDER
   if ( digitalRead(shoulderRSensor) ) 
    turnOnG();
   else 
   turnOffLEDS();
    
   // NECK
   if ( digitalRead(neckSensor) ) 
    turnOnH();
   else 
   turnOffLEDS();
  }
  
  if (!badPosture) {  //good posture, so turn off all LED indicator lights
  turnOffLEDS(); 
  }

}


//**************************************************************************************
//**************************************************************************************
//                               FUNCTIONS AND INTERRUPTS

//**************************************************************************************
//                               INTERRUPT SERVICE HANDLERS

void postureHandler(){
  badPosture = digitalRead(badPosturePin);
  if(badPosture){
      Serial.println("bad");
      TCNT1 = 0; //Reset timer1 so that the count for bad posture can restart
      TIMSK1 = 6; //enable OCR1A/B (timer compare interrupts)
  }
 if(!badPosture) {
      Serial.println("good");
      TIMSK1 = 0; //disable timer compare interrupts
      timerP1 = false;
      timerP2 = false;
 }
}

ISR (TIMER1_COMPA_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("1.5s");
  timerP1 = true;
}

ISR (TIMER1_COMPB_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("3s");
  timerP2 = true;
}

ISR(TIMER1_OVF_vect) {
  timer1_ovf++;
}

//**************************************************************************************
//                               TIMER FUNCTIONS

float timer1Millis(){
  //outputs runtime since reset in ms, for prescaler of 1024
  //16Mhz/1024 = 15625
  //1/15625 = 0.064ms (each event represents 0.064ms)
  //timer1 holds 65535 events
  //each overflow represents 4194.24ms
  
  float time = (float) timer1_ovf*(4194.24) + TCNT1*(0.064);
  return time;
}


//**************************************************************************************
//                               SENSOR FUNCTIONS


//**************************************************************************************
//                               LED FUNCTIONS
void turnOnA(){
  //turn only 'a' on
  digitalWrite (ledPinC1, HIGH);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH); 
    delay(delayTime);
}

void turnOnB(){
  //turn only 'b' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, HIGH);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH); 
    delay(delayTime);
}

void turnOnC(){
  //turn only 'c' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, HIGH);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH); 
    delay(delayTime);
}

void turnOnD(){
  //turn only 'd' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, HIGH);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH); 
}

void turnOnE(){
  //turn only 'e' on
  digitalWrite (ledPinC1, HIGH);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW); 
    delay(delayTime);
}

void turnOnF(){
  //turn only 'f' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, HIGH);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW); 
    delay(delayTime);
}

void turnOnG(){
  //turn only 'g' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, HIGH);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW); 
    delay(delayTime);
}

void turnOnH(){
  //turn only 'h' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, HIGH);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW); 
    delay(delayTime);
}

void turnOffLEDS(){
  //turn oall LEDS off
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, LOW); 
    delay(delayTime);
}
//**************************************************************************************

