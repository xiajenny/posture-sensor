

int potentiometer = A0;    // select the input pin for the potentiometer
int flex = A1;    // select the input pin for the potentiometer
int ledPin = 3;   // select the pin for the LED
int pval = 0;       // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
}

void loop() {
  pval = analogRead(potentiometer);    // read the value from the sensor
  Serial.println("potentiometer: ");
  Serial.print(pval);

  if (pval > 700) {
    //badPosturePin = 1;
Serial.println("light up : ");
    digitalWrite(3, HIGH);
//    delay(500);
  }
  else if (pval < 300) {
    //badPosturePin = 1;
Serial.println("light up : ");
//delay(500);
    digitalWrite(3, HIGH);
  }
  else if (pval > 300 && pval < 700) {
    //badPosturePin = 0;
Serial.println("don't light up : ");
//delay(500);
    digitalWrite(3, LOW);
   
  }                 // stop the program for some time
}
