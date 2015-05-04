
unsigned count0=0;
unsigned count1=0;
unsigned count2=0;
unsigned count3=0;
unsigned count4=0;
unsigned count5=0;

unsigned overflow0=125;
unsigned overflow1=100;
unsigned overflow2=83;
unsigned overflow3=71;
unsigned overflow4=63;
unsigned overflow5=125;

unsigned half0=63;
unsigned half1=50;
unsigned half2=42;
unsigned half3=36;
unsigned half4=31;
unsigned half5=63;

void setup() {
  // put your setup code here, to run once:
  bitSet(DDRB,0);
  bitSet(DDRB,1);
  bitSet(DDRB,2);
  bitSet(DDRB,3);
  bitSet(DDRB,4);
  bitSet(DDRB,5);
  
  TCCR1A=0;
  TCCR1B=1<<WGM13|1<<WGM12|1<<CS11|1<<CS10;
  ICR1=0;
  bitSet(TIMSK1,ICIE1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR(TIMER1_CAPT_vect){
  //counter0
  count0++;
  if(count0==half0){
    bitSet(PINB,0);
  }
  if(count0==overflow0){
    bitSet(PINB,0);
    count0=0;
  }
  //counter1
  count1++;
  if(count1==half1){
    bitSet(PINB,1);
  }
  if(count1==overflow1){
    bitSet(PINB,1);
    count1=0;
  }
  //counter2
  count2++;
  if(count2==half2){
    bitSet(PINB,2);
  }
  if(count2==overflow2){
    bitSet(PINB,2);
    count2=0;
  }
  //counter3
  count3++;
  if(count3==half3){
    bitSet(PINB,3);
  }
  if(count3==overflow3){
    bitSet(PINB,3);
    count3=0;
  }
  //counter4
  count4++;
  if(count4==half4){
    bitSet(PINB,4);
  }
  if(count4==overflow4){
    bitSet(PINB,4);
    count4=0;
  }
  //counter5
  count5++;
  if(count5==half5){
    bitSet(PINB,5);
  }
  if(count5==overflow5){
    bitSet(PINB,5);
    count5=0;
  }
}
