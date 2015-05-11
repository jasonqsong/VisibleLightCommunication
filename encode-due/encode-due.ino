#define LIGHT_MAX 5
#define CHANNEL_MAX 4
#define DIGITAL_PIN_MAX 54
unsigned code[LIGHT_MAX][CHANNEL_MAX];
unsigned port[LIGHT_MAX];
unsigned count=0;
unsigned overflow=2;
unsigned half=1;
unsigned b;
unsigned pinStat[DIGITAL_PIN_MAX];

void setPin(unsigned pin){
  digitalWrite(pin,HIGH);
  pinStat[pin]=1;
}
void clearPin(unsigned pin){
  digitalWrite(pin,LOW);
  pinStat[pin]=0;
}
void invertPin(unsigned pin){
  if(pinStat[pin]&1){
    clearPin(pin);
  }else{
    setPin(pin);
  }
}

void TC0_Handler()
{
  unsigned stat=REG_TC0_SR0; // TC Status Register (cleared on read)
  unsigned i,j;
  count=count+1;
  if(count==half){
    for(i=0;i<LIGHT_MAX;i=i+1){
      for(j=0;j<CHANNEL_MAX;j=j+1){
        invertPin(port[i]+j);
      }
    }
  }
  if(count==overflow){
    count=0;
    if(b==0x80)
      b=0x01;
    else
      b=b<<1;
    for(i=0;i<LIGHT_MAX;i=i+1){
      for(j=0;j<CHANNEL_MAX;j=j+1){
        if(code[i][j]&b){
          setPin(port[i]+j);
        }else{
          clearPin(port[i]+j);
        }
      }
    }
  }
}
void clearStat(){
  unsigned i;
  for(i=0;i<DIGITAL_PIN_MAX;i=i+1){
    pinStat[i]=0;
  }
}
void initCode(){
  unsigned i,j;
  for(i=0;i<LIGHT_MAX;i=i+1){
    for(j=0;j<CHANNEL_MAX;j=j+1){
      code[i][j]=0;
    }
    code[i][0]=0xff;
    code[i][1]=0xf0;
  }
  code[0][2]=0xcc;
  code[1][2]=2;
  code[2][2]=3;
  code[3][2]=4;
  code[4][2]=5;
  port[0]=4;
  port[1]=8;
  port[2]=12;
  port[3]=16;
  port[4]=20;
}
void setupPio(){
  unsigned i;
  for(i=0;i<DIGITAL_PIN_MAX;i=i+1){
    pinMode(i,OUTPUT);
  }
}
void setupTimer(){
  pinMode(2,OUTPUT); // port B pin 25  
  analogWrite(2,255); // sets up some other registers I haven't worked out yet
  REG_PIOB_PDR = 0x1u<<25; // disable PIO, enable peripheral
  REG_PIOB_ABSR= 0x1u<<25; // select peripheral B

  REG_TC0_WPMR=0x54494D00; // enable write to registers
  REG_TC0_CCR0=TC_CCR_CLKEN|TC_CCR_SWTRG; //REG_TC0_CCR0=0b101; // start counter
  REG_TC0_CMR0=TC_CMR_TCCLKS_TIMER_CLOCK1|TC_CMR_EEVT_XC0|TC_CMR_WAVSEL_UP_RC|TC_CMR_WAVE|TC_CMR_ACPA_SET|TC_CMR_ACPC_CLEAR; //REG_TC0_CMR0=0b0000,0000,0000,1001,1100,0100,0000,0000; // set channel mode register (see datasheet)

  REG_TC0_RC0=20000; // counter period
  
  REG_TC0_IER0=TC_IER_CPCS; // REG_TC0_IER0=0b00010000; // enable interrupt on counter=rc
  REG_TC0_IDR0=~TC_IDR_CPCS; // REG_TC0_IDR0=0b11101111; // disable other interrupts

  NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts
}
    
void setup() {
  clearStat();
  initCode();
  setupPio();
  setupTimer();
  b=0x01;
}

void loop() {
}

