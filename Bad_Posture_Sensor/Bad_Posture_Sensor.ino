int badPosturePin = 2; //button/pin to indicate if bad posture (when LOW/pressed)

int beginnerLED = 10;  //beginner (slight) bad posture - lights up immediately
int intermediateLED = 11;  //intermediate bad posture - lights up after 15 seconds
int extremeLED = 12; //extremely bad posture - lights up after 30 seconds

int backPin = 7;
int shoulderPin = 8;
int spinePin = 9;

int backSensor = 4;
int shoulderSensor = 5;
int spineSensor = 6;

int flexSensor = A1;
float value = 0;  //change to int? analogRead() changes to voltage to int between 0-1023

volatile bool badPosture = 0; //determined by state of badPosture pin

void setup() {
  Serial.begin(9600);     //enable serial monitor
  DDRB |= (1 << 5);       //enable pin 13
  
  pinMode(badPosturePin, INPUT_PULLUP);  //but change to pull down if that's a thing
  
  pinMode(beginnerLED, OUTPUT);
  pinMode(intermediateLED, OUTPUT);
  pinMode(extremeLED, OUTPUT);

  pinMode(backPin, OUTPUT);
  pinMode(shoulderPin, OUTPUT);
  pinMode(spinePin, OUTPUT);

  pinMode(backSensor,INPUT);
  pinMode(shoulderSensor, INPUT);
  pinMode(spineSensor, INPUT);

  pinMode(3, OUTPUT);

  
  TCCR1A = 0;     //reset  
  TCCR1B = 0;
  TCCR1B = 5;

  attachInterrupt(digitalPinToInterrupt(badPosturePin), postureHandler, CHANGE);  

  OCR1A = 23437; //1.5 seconds and LED flashes
  OCR1B = 46875; //3 seconds and LED flashes

}

void loop() {
value = analogRead(flexSensor);
Serial.println("Flex value: ");
Serial.println(value);  
delay(500);


  if (value > float(450)) {
    //badPosturePin = 1;
Serial.println("light up : ");
    digitalWrite(3, HIGH);
//    delay(500);
  }
  else if (value < float(300)) {
    //badPosturePin = 1;
Serial.println("light up : ");
//delay(500);
    digitalWrite(3, HIGH);
  }
  else if (value > float(300) && value < float(450)) {
    //badPosturePin = 0;
Serial.println("don't light up : ");
//delay(500);
    digitalWrite(3, LOW);
   
  }
  
  if (badPosture) {
   digitalWrite(beginnerLED, HIGH);
   if ( digitalRead(backSensor) ) 
    digitalWrite(backPin, HIGH);
   else 
    digitalWrite(backPin, LOW);
    
   if ( digitalRead(shoulderSensor) ) 
    digitalWrite(shoulderPin,HIGH);
   else 
    digitalWrite(shoulderPin, LOW);
    
   if ( digitalRead(spineSensor) ) 
    digitalWrite(spinePin, HIGH);
   else 
    digitalWrite(spinePin, LOW);
  }
  
  if (!badPosture) {  //good Posture
   digitalWrite(beginnerLED, LOW);
   digitalWrite(intermediateLED, LOW);
   digitalWrite(extremeLED, LOW);

   digitalWrite(backPin, LOW);
   digitalWrite(shoulderPin, LOW);
   digitalWrite(spinePin, LOW);
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
 
}

ISR (TIMER1_COMPA_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("1.5s");
  digitalWrite(intermediateLED, HIGH);

}

ISR (TIMER1_COMPB_vect) { //intermediate bad posture, after 100 seconds
  Serial.println("3s");
  digitalWrite(extremeLED, HIGH);
}




