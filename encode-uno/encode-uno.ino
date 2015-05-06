unsigned code[4];
unsigned count=0;
unsigned overflow=125;
unsigned half=63;
unsigned b;

void setup() {
  // put your setup code here, to run once:
  code[0]=0xff;
  code[1]=0x01;
  code[2]=3;
  code[3]=0;
  b=0x01;
  
  bitSet(DDRB,0);
  bitSet(DDRB,1);
  bitSet(DDRB,2);
  bitSet(DDRB,3);
  
  TCCR1A=0;
  TCCR1B=1<<WGM13|1<<WGM12|1<<CS11|1<<CS10;
  ICR1=0;
  bitSet(TIMSK1,ICIE1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR(TIMER1_CAPT_vect){
  count=count+1;
  if(count==half){
    bitSet(PINB,0);
    bitSet(PINB,1);
    bitSet(PINB,2);
    bitSet(PINB,3);
  }
  if(count==overflow){
    if(b==0x80)
      b=0x01;
    else
      b=b<<1;
    if(code[0]&b)
      bitSet(PORTB,0);
    else
      bitClear(PORTB,0);
    if(code[1]&b)
      bitSet(PORTB,1);
    else
      bitClear(PORTB,1);
    if(code[2]&b)
      bitSet(PORTB,2);
    else
      bitClear(PORTB,2);
    if(code[3]&b)
      bitSet(PORTB,3);
    else
      bitClear(PORTB,3);
  }
}
