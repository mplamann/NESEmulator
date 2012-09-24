/*
Description:	Interfacing a NES controller with a PC with an Arduino.
Coded by:	Prodigity
Date:		1 December 2011
Revision:	V0.92 (beta)
*/

const int latch = 17;
const int clock = 16;
const int data  = 18;

#define latchlow digitalWrite(latch, LOW)
#define latchhigh digitalWrite(latch, HIGH)
#define clocklow digitalWrite(clock, LOW)
#define clockhigh digitalWrite(clock, HIGH)
#define dataread digitalRead(data)
#define wait delayMicroseconds(200)

byte output;

void setup() {
	Serial.begin(9600);
	pinMode(latch, OUTPUT);
        pinMode(clock, OUTPUT);
        pinMode(data, INPUT);
}

void loop() {
  output = 0;
  ReadNESjoy();
  //Serial.print(output, BYTE);
  Serial.println(output);
}


void ReadNESjoy() {
  latchlow;
  clocklow;
  latchhigh;
  wait;
  latchlow;
  for (int i = 0; i < 8; i++) {
     clockhigh;
     wait;
     output += dataread * (1 << i);
     clocklow;
     wait;
  }
}
