int badPosturePin = 2; //button/pin to indicate if bad posture (when HIGH/pressed)
int beginnerLED = 10;  //beginner (slight) bad posture - lights up immediately
int intermediateLED = 11;  //intermediate bad posture - lights up after 15 seconds
int extremeLED = 12; //extremely bad posture - lights up after 30 seconds
volatile bool badPosture = 0; //determined by state of badPosture pin

void setup() {
  Serial.begin(9600);
  DDRB |= (1 << 5);
  pinMode(badPosturePin, INPUT_PULLUP);  //but change to pull down if that's a thing
  pinMode(beginnerLED, OUTPUT);
  pinMode(intermediateLED, OUTPUT);
  pinMode(extremeLED, OUTPUT);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = 3;

  attachInterrupt(digitalPinToInterrupt(badPosturePin), postureHandler, CHANGE);  

  TIMSK1 = 6; //enable OCR1A/B
  OCR1A = 100; //100 seconds and LED flashes
  OCR1B = 1000; //1000 seconds and LED flashes

}

void loop() {
  if (badPosture) {
   digitalWrite(beginnerLED, HIGH);
  }
  
  if (!badPosture) {  //goodPosture
   TCCR1B = 0; //pause time
   digitalWrite(beginnerLED, LOW);
   digitalWrite(intermediateLED, LOW);
   digitalWrite(extremeLED, LOW);
  
  }

}

void postureHandler(){
  Serial.println("bad");
  badPosture = digitalRead(badPosturePin);
  TCNT1 = 0;  //start counting time to determine how long bad posture has been maintained
  return;
}

ISR (TIMER1_COMPA_vect) { //intermediate bad posture, after 100 seconds
  digitalWrite(intermediateLED, HIGH);
  //insert delay?
}

ISR (TIMER1_COMPB_vect) { //intermediate bad posture, after 100 seconds
  digitalWrite(extremeLED, HIGH);
}




