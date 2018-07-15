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

//Flags for overall timing lights (first row of LEDS)
volatile bool timerP1 = false;
volatile bool timerP2 = false;
volatile bool timerP3 = false;

//Overall bad posture timing variables
volatile bool startTime = false;
volatile bool endTime = false;
float startOverallTime = 0;
float duration = 0;

//Individual bad posture timer variables
bool startBack = false;
float startTimeBack = 0.0;
float totalBackTime = 0.0;

bool startShoulderL = false;
float startTimeShoulderL = 0.0;
float totalShoulderLTime = 0.0;

bool startShoulderR = false;
float startTimeShoulderR = 0.0;
float totalShoulderRTime = 0.0;

bool startNeck = false;
float startTimeNeck = 0.0;
float totalNeckTime = 0.0;


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
  cli();
  //TIMERS & INTERRUPTS
  TCCR1A = 0;  //reset
  TCCR1B = 0;  //reset
  TCCR1B = 5;  //set timer1 prescaler to /1024
  TIMSK1 = 1; //enable overflow interrupt for timer1 (used for our timer1Millis())

  //Whenever either type of bad posture is detected, the badPosturePin in will turn HIGH
  attachInterrupt(digitalPinToInterrupt(badPosturePin), postureHandler, CHANGE);

  OCR1A = 23437; //timer1 compare interruptA value: 1.5 seconds and LED flashes
  OCR1B = 46875; //timer1 compare interruptB value: 3 seconds and LED flashes

  //SENSORS
  pinMode(badPosturePin, INPUT_PULLUP);
  pinMode(backSensor, INPUT);
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

  sei();
}

//**************************************************************************************
//**************************************************************************************
//                              MAIN LOOP

void loop() {

  if (startTime) {
    startOverallTime = timer1Millis();  //set start time for timing overall bad posture
    startTime = false;                  //reset flag
  }

  if (endTime) {
    duration = (timer1Millis() - startOverallTime) / 1000; //calculate duration in s
    //Printing recorded timings
    Serial.print("Overall bad posture lasted for: ");
    Serial.print(duration);
    Serial.println("s");
    Serial.print("Back time: ");
    Serial.println(totalBackTime);
    Serial.print("Left Shoulder time: ");
    Serial.println(totalShoulderLTime);
    Serial.print("Right Shoulder time: ");
    Serial.println(totalShoulderRTime);
    Serial.print("Neck time: ");
    Serial.println(totalNeckTime);
    totalBackTime = 0;
    totalShoulderLTime = 0;
    totalShoulderRTime = 0;
    totalNeckTime = 0;
    endTime = false;      //reset flag
  }

  //The following 4 if statements turn on the "timing indicator" lights (top row of LEDs)
  if (badPosture) {
    turnOnA();
    if ( timerP1 )
      turnOnB();
    if ( timerP2 )
      turnOnC();
    if ( timerP3 )
      turnOnD();

    //The following 4 if statements turn on the "target problem area indicator" lights
    //                                                             (bottom row of LEDS)
    // BACK
    if ( digitalRead(backSensor) ) {
      if (!startBack) {
        startTimeBack = timer1Millis();
        startBack = true;
      }
      turnOnE();
    }
    else {
      turnOffLEDS();
      if (startBack) {
        totalBackTime = ((timer1Millis() - startTimeBack ) + totalBackTime) / 1000;
        startBack = false;
        Serial.println( totalBackTime );
      }
    }

    // LEFT SHOULDER
    if ( digitalRead(shoulderLSensor) ) {
      if (!startShoulderL) {
        startTimeShoulderL = timer1Millis();
        startShoulderL = true;
      }
      turnOnF();
    }
    else {
      turnOffLEDS();
      if (startShoulderL) {
        totalShoulderLTime = ((timer1Millis() - startTimeShoulderL ) + totalShoulderLTime) / 1000;
        startShoulderL = false;
        Serial.println( totalShoulderLTime );
      }
    }

    // RIGHT SHOULDER
    if ( digitalRead(shoulderRSensor) ) {
      if (!startShoulderR) {
        startTimeShoulderR = timer1Millis();
        startShoulderR = true;
      }
      turnOnG();
    }
    else {
      turnOffLEDS();
      if (startShoulderR) {
        totalShoulderRTime = ((timer1Millis() - startTimeShoulderR ) + totalShoulderRTime) / 1000;
        startShoulderR = false;
        Serial.println( totalShoulderRTime );
      }
    }

    // NECK
    if ( digitalRead(neckSensor) ) {
      if (!startNeck) {
        startTimeNeck = timer1Millis();
        startNeck = true;
      }
      turnOnH();
    }
    else {
      turnOffLEDS();
      if (startNeck) {
        totalNeckTime = ((timer1Millis() - startTimeNeck ) + totalNeckTime) / 1000;
        startNeck = false;
        Serial.println( totalNeckTime );
      }
    }

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

void postureHandler() {
  badPosture = digitalRead(badPosturePin);
  if (badPosture) {
    Serial.println("bad");
    TCNT1 = 0; //Reset timer1 so that the count for bad posture can restart
    TIMSK1 = 7; //enable OCR1A/B (timer compare interrupts) as well as timer overflow interrupt
    startTime = true;
    endTime = false;
  }
  if (!badPosture) {
    Serial.println("good");
    TIMSK1 = 1; //disable timer compare interrupts, keep timer overflow interrupt enabled
    timerP1 = false;
    timerP2 = false;
    timerP3 = false;
    startTime = false;
    endTime = true;
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

ISR (TIMER1_OVF_vect) {
  Serial.println("overflow");
  timer1_ovf++;
  timerP3 = true;
}

//**************************************************************************************
//                               TIMER FUNCTIONS

float timer1Millis() {
  //outputs runtime since reset in ms, for prescaler of 1024
  //16Mhz/1024 = 15625
  //1/15625 = 0.064ms (each event represents 0.064ms)
  //timer1 holds 65535 events
  //each overflow represents 4194.24ms

  float time = (float) timer1_ovf * (4194.24) + TCNT1 * (0.064);
  return time;
}


//**************************************************************************************
//                               SENSOR FUNCTIONS


//**************************************************************************************
//                               LED FUNCTIONS
void turnOnA() {
  //turn only 'a' on
  digitalWrite (ledPinC1, HIGH);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH);
  delay(delayTime);
}

void turnOnB() {
  //turn only 'b' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, HIGH);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH);
  delay(delayTime);
}

void turnOnC() {
  //turn only 'c' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, HIGH);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH);
  delay(delayTime);
}

void turnOnD() {
  //turn only 'd' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, HIGH);
  digitalWrite (ledPinR1, LOW);
  digitalWrite (ledPinR2, HIGH);
}

void turnOnE() {
  //turn only 'e' on
  digitalWrite (ledPinC1, HIGH);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW);
  delay(delayTime);
}

void turnOnF() {
  //turn only 'f' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, HIGH);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW);
  delay(delayTime);
}

void turnOnG() {
  //turn only 'g' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, HIGH);
  digitalWrite (ledPinC4, LOW);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW);
  delay(delayTime);
}

void turnOnH() {
  //turn only 'h' on
  digitalWrite (ledPinC1, LOW);
  digitalWrite (ledPinC2, LOW);
  digitalWrite (ledPinC3, LOW);
  digitalWrite (ledPinC4, HIGH);
  digitalWrite (ledPinR1, HIGH);
  digitalWrite (ledPinR2, LOW);
  delay(delayTime);
}

void turnOffLEDS() {
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

