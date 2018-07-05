int badPosturePin = 2; //button/pin to indicate if bad posture (when LOW/pressed)
int beginnerLED = 10;  //beginner (slight) bad posture - lights up immediately
int intermediateLED = 11;  //intermediate bad posture - lights up after 15 seconds
int extremeLED = 12; //extremely bad posture - lights up after 30 seconds
volatile bool badPosture = 0; //determined by state of badPosture pin

void setup() {
  Serial.begin(9600);     //enable serial monitor
  DDRB |= (1 << 5);
  pinMode(badPosturePin, INPUT_PULLUP);  //but change to pull down if that's a thing
  pinMode(beginnerLED, OUTPUT);
  pinMode(intermediateLED, OUTPUT);
  pinMode(extremeLED, OUTPUT);
  TCCR1A = 0;     //reset  
  TCCR1B = 0;
  TCCR1B = 5;

  attachInterrupt(digitalPinToInterrupt(badPosturePin), postureHandler, CHANGE);  

  OCR1A = 23437; //1.5 seconds and LED flashes
  OCR1B = 46875; //3 seconds and LED flashes

}

void loop() {
  if (badPosture) {
   digitalWrite(beginnerLED, HIGH);
  }
  
  if (!badPosture) {  //good Posture
   digitalWrite(beginnerLED, LOW);
   digitalWrite(intermediateLED, LOW);
   digitalWrite(extremeLED, LOW);
  }

}

void postureHandler(){
  badPosture = digitalRead(badPosturePin);
  if(badPosture){
      Serial.println("bad");
      TCNT1 = 0;
      TIMSK1 = 6; //enable OCR1A/B
  }
 if(!badPosture) {
      Serial.println("good");
      TIMSK1 = 0;   
 }
  
  //start counting time to determine how long bad posture has been maintained
  return;
}

ISR (TIMER1_COMPA_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("1.5s");
  digitalWrite(intermediateLED, HIGH);

}

ISR (TIMER1_COMPB_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("3s");
  digitalWrite(extremeLED, HIGH);
}




