#define DP00P PINA
#define DP00B 8
#define DP01P PINA
#define DP01B 9
#define DP02P PINB
#define DP02B 25
#define DP03P PINC
#define DP03B 28
#define DP04P PINA
#define DP04B 29
#define DP05P PINC
#define DP05B 25
#define DP06P PINC
#define DP06B 24
#define DP07P PINC
#define DP07B 23
#define DP08P PINC
#define DP08B 22
#define DP09P PINC
#define DP09B 21

#define DP10P PINA
#define DP10B 28
#define DP11P PIND
#define DP11B 7
#define DP12P PIND
#define DP12B 8
#define DP13P PINB
#define DP13B 27
#define DP14P PIND
#define DP14B 4
#define DP15P PIND
#define DP15B 5
#define DP16P PINA
#define DP16B 13
#define DP17P PINA
#define DP17B 12
#define DP18P PINA
#define DP18B 11
#define DP19P PINA
#define DP19B 10

#define DP20P PINB
#define DP20B 12
#define DP21P PINB
#define DP21B 13
#define DP22P PINB
#define DP22B 26
#define DP23P PINA
#define DP23B 14
#define DP24P PINA
#define DP24B 15
#define DP25P PIND
#define DP25B 0
#define DP26P PIND
#define DP26B 1
#define DP27P PIND
#define DP27B 2
#define DP28P PIND
#define DP28B 3
#define DP29P PIND
#define DP29B 6

#define DP30P PIND
#define DP30B 9
#define DP31P PINA
#define DP31B 7
#define DP32P PIND
#define DP32B 10
#define DP33P PINC
#define DP33B 1
#define DP34P PINC
#define DP34B 2
#define DP35P PINC
#define DP35B 3
#define DP36P PINC
#define DP36B 4
#define DP37P PINC
#define DP37B 5
#define DP38P PINC
#define DP38B 6
#define DP39P PINC
#define DP39B 7

#define DP40P PINC
#define DP40B 8
#define DP41P PINC
#define DP41B 9
#define DP42P PINA
#define DP42B 19
#define DP43P PINA
#define DP43B 20
#define DP44P PINC
#define DP44B 19
#define DP45P PINC
#define DP45B 18
#define DP46P PINC
#define DP46B 17
#define DP47P PINC
#define DP47B 16
#define DP48P PINC
#define DP48B 15
#define DP49P PINC
#define DP49B 14

#define DP50P PINC
#define DP50B 13
#define DP51P PINC
#define DP51B 12
#define DP52P PINB
#define DP52B 21
#define DP53P PINB
#define DP53B 14

unsigned code[5][4];
unsigned count=0;
unsigned overflow=125;
unsigned half=63;
unsigned i,j;
unsigned b;

void setup() {
  // put your setup code here, to run once:
  for(i=0;i<5;i=i+1){
    for(j=0;j<4;j=j+1){
      code[i][j]=0;
    }
  }
  for(i=0;i<5;i=i+1){
    code[i][0]=0xff;
    code[i][1]=0x01;
  }
  code[0][2]=1;
  code[1][2]=2;
  code[2][2]=3;
  code[3][2]=4;
  code[4][2]=5;
  b=0x01;
  
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
  count=count+1;
  if(count==half){
    
  }
  if(count==overflow){
  }
}
/*
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
*/

