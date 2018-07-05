unsigned long int overflows = 0;


void setup() {
cli();
TCCR1A = 0;     //reset  
TCCR1B = 0;
TCCR1B = 5;
TIMSK1 = 0x1;
sei();

Serial.begin(9600);

}

void loop() {
  Serial.println(timer1Millis()/1000);

}

float timer1Millis(){
  //outputs runtime since reset in ms, for prescaler of 1024
  //16Mhz/1024 = 15625
  //1/15625 = 0.064ms (each event represents 0.064ms)
  //timer1 holds 65535 events
  //each overflow represents 4194.24ms
  
  float time = (float) overflows*(4194.24) + TCNT1*(0.064);
  return time;
}


ISR(TIMER1_OVF_vect){
  overflows++;
  Serial.print("overflow");
  Serial.println(overflows);
}

