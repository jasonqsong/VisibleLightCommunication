/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
unsigned n;
unsigned x;
unsigned pin;
unsigned refpin;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pin=13;
  pinMode(pin, OUTPUT);
  refpin=12;
  pinMode(refpin, OUTPUT);
  n=8;
  x=0;
}
void encode(unsigned ch){
  unsigned l=0x8;
  digitalWrite(refpin,HIGH);
  while(l){
    if(ch&l)
      digitalWrite(pin,HIGH);
    else
      digitalWrite(pin,LOW);
    delay(1);
    l=l>>1;  
    digitalWrite(refpin,LOW);
  }
}
// the loop function runs over and over again forever
void loop() {              // wait for a second
  if((++x)>=n)
    x=0;
  encode(1);
}
