/*
Description:	Interfacing a NES controller with a PC with an Arduino.
Coded by:	Prodigity
Date:		1 December 2011
Revision:	V0.92 (beta)
*/

const int latch = 7;
const int clock = 5;
const int data  = 6;

#define DELAY 2

byte output;

void setup() {
	Serial.begin(9600);
	pinMode(latch, OUTPUT);
        pinMode(clock, OUTPUT);
        pinMode(data, INPUT);
        pinMode(13,OUTPUT);
}

void loop() {
  output = 0;
  ReadNESjoy();
  //Serial.print(output, BYTE);
  Serial.write(output);
}


void ReadNESjoy() {
  digitalWrite(latch, LOW);
  digitalWrite(clock, LOW);
  digitalWrite(latch, HIGH);
  delayMicroseconds(DELAY);
  digitalWrite(latch, LOW);
  for (int i = 0; i < 8; i++) {
     digitalWrite(clock, HIGH);
     delayMicroseconds(DELAY);
     output += digitalRead(data) * (1 << i);
     digitalWrite(clock, LOW);
     delayMicroseconds(DELAY);
     if (i == 6 && digitalRead(data))
       digitalWrite(13, HIGH);
     else if (i == 6);
       digitalWrite(13,LOW);
  }
}
