unsigned i=0;
void TC0_Handler()
{
  unsigned stat=REG_TC0_SR0; //TC Status Register (cleared on read)
  if(i&1)
    digitalWrite(13,LOW);
  else
    digitalWrite(13,HIGH);
  i=i+1;
}

void setup(){
  pinMode(13,OUTPUT);
  // set pin mode
  pinMode(2,OUTPUT);    // port B pin 25  
  analogWrite(2,255);   // sets up some other registers I haven't worked out yet
  REG_PIOB_PDR = 0x1u<<25; // disable PIO, enable peripheral
  REG_PIOB_ABSR= 0x1u<<25; // select peripheral B

  REG_TC0_WPMR=0x54494D00; // enable write to registers
  REG_TC0_CCR0=TC_CCR_CLKEN|TC_CCR_SWTRG; //REG_TC0_CCR0=0b101; // start counter
  REG_TC0_CMR0=TC_CMR_TCCLKS_TIMER_CLOCK1|TC_CMR_EEVT_XC0|TC_CMR_WAVSEL_UP_RC|TC_CMR_WAVE|TC_CMR_ACPA_SET|TC_CMR_ACPC_CLEAR; //REG_TC0_CMR0=0b0000,0000,0000,1001,1100,0100,0000,0000; // set channel mode register (see datasheet)

  REG_TC0_RC0=10000000; // counter period
  //REG_TC0_RA0=30000000;  // PWM value
  
  REG_TC0_IER0=TC_IER_CPCS; //REG_TC0_IER0=0b00010000; // enable interrupt on counter=rc
  REG_TC0_IDR0=~TC_IDR_CPCS; // REG_TC0_IDR0=0b11101111; // disable other interrupts

  NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts
  /*
  pinMode(13,OUTPUT);
  pinMode(2,OUTPUT);    // port B pin 25  
  analogWrite(2,255);   // sets up some other registers I haven't worked out yet
  REG_PIOB_PDR = 1<<25; // disable PIO, enable peripheral
  REG_PIOB_ABSR= 1<<25; // select peripheral B
  REG_TC0_WPMR=0x54494D00; // enable write to registers
  REG_TC0_CMR0=TC_CMR_TCCLKS_TIMER_CLOCK1|TC_CMR_EEVT_XC0|TC_CMR_WAVSEL_UP_RC|TC_CMR_WAVE|TC_CMR_ACPA_SET|TC_CMR_ACPC_CLEAR; //REG_TC0_CMR0=0b0000,0000,0000,1001,1100,0100,0000,0000; // set channel mode register (see datasheet)
  REG_TC0_RC0=100000000; // counter period
  REG_TC0_RA0=30000000;  // PWM value
  
  REG_TC0_CCR0=TC_CCR_CLKEN|TC_CCR_SWTRG; //REG_TC0_CCR0=0b101; // start counter
  REG_TC0_IER0=0b00010000; // enable interrupt on counter=rc
  REG_TC0_IDR0=0b11101111; // disable other interrupts

  NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts
  */
}

void loop(){
}
