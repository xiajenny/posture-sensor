int badPosturePin = 2; //button/pin to indicate if bad posture (when HIGH/pressed)
int beginnerLED = 10;  //beginner (slight) bad posture - lights up immediately
int intermediateLED = 11;  //intermediate bad posture - lights up after 100 seconds
int extremeLED = 12; //extremely bad posture - lights up after 1000 seconds
bool badPosture = 0; //determined by state of badPosture pin

void setup() {
DDRB |= (1 << 5);
pinMode(badPosturePin, INPUT);  //but change to pull down if that's a thing
pinMode(beginnerLED, OUTPUT);
pinMode(intermediateLED, OUTPUT);
pinMode(extremeLED, OUTPUT);

TCCR1A = 0;
TCCR1B = 0;
TCCR1B = 3;

attachInterrupt(digitalPinToInterrupt(badPosturePin), handler(), HIGH);  //bad posture when button is high

TIMSK1 = 6; //enable OCR1A/B
OCR1A = 100; //100 seconds and LED flashes
OCR1B = 1000; //1000 seconds and LED flashes

}

void loop() {
badPosture = digitalread(badPosturePin);  //read in handler?

if (!badPosture) {  //goodPosture
  TCCR1B = 0; //pause time
  digitalWrite(beginnerLED, LOW);
  digitalWrite(intermediateLED, LOW);
  digitalWrite(extremeLED, LOW);
  
}


}

void handler(){
  digitalWrite(beginnerLED, HIGH);  //light up first LED indicating change from good posture to bad posture
  TCNT1 = 0;  //start counting time to determine how long bad posture has been maintained
  //reset badPosture bool to 0?
}

ISR (TIMER1_COMPA_vect) { //intermediate bad posture, after 100 seconds
  digitalWrite(intermediateLED, HIGH);
  //insert delay?
  
}

ISR (TIMER1_COMPB_vect) { //intermediate bad posture, after 100 seconds
  digitalWrite(extremeLED, HIGH);
  
}




