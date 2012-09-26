/*
Description:	Interfacing a NES controller with a PC with an Arduino.
Coded by:	Prodigity
Date:		1 December 2011
Revision:	V0.92 (beta)
*/

const int latch = 17;
const int clock = 16;
const int data  = 18;

<<<<<<< HEAD
#define latchlow digitalWrite(latch, LOW)
#define latchhigh digitalWrite(latch, HIGH)
#define clocklow digitalWrite(clock, LOW)
#define clockhigh digitalWrite(clock, HIGH)
#define dataread digitalRead(data)
#define wait delayMicroseconds(200)
=======
#define DELAY 2
>>>>>>> c5302fd116d6e2a57b9c5755aec3a52d48b71315

byte output;

void setup() {
	Serial.begin(9600);
	pinMode(latch, OUTPUT);
        pinMode(clock, OUTPUT);
        pinMode(data, INPUT);
<<<<<<< HEAD
=======
        pinMode(13,OUTPUT);
>>>>>>> c5302fd116d6e2a57b9c5755aec3a52d48b71315
}

void loop() {
  output = 0;
  ReadNESjoy();
  //Serial.print(output, BYTE);
<<<<<<< HEAD
  Serial.println(output);
=======
  Serial.write(output);
>>>>>>> c5302fd116d6e2a57b9c5755aec3a52d48b71315
}


void ReadNESjoy() {
<<<<<<< HEAD
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
=======
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
>>>>>>> c5302fd116d6e2a57b9c5755aec3a52d48b71315
  }
}
