int badPosture = 5; //pin to indicate if bad posture (when HIGH/pressed)

void setup() {
DDRB |= (1 << 5);
pinMode(badPosture, INPUT_PULLUP);

TCCR1A = 0;
TCCR1B = 0;
TCCR1B = 3;

attachInterrupt(digitalPinToInterrupt(badPosture), handler(), HIGH);

TIMSK1 = 6;
OCR1A = 100; //100 seconds and LED flashes
OCR1B = 1000; //1000 seconds and LED flashes

}

void loop() {
  // put your main code here, to run repeatedly:

}
