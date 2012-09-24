/* INITIALIZATION */

int latch = 17; // set the latch pin
int clock = 16; // set the clock pin
int datin = 18;// set the data in pin
byte controller_data = 0;
int ledpin = 13;


/* SETUP */
void setup() {
Serial.begin(9600);
pinMode(latch,OUTPUT);
pinMode(clock,OUTPUT);
pinMode(datin,INPUT);
pinMode(ledpin, OUTPUT);

digitalWrite(latch,HIGH);
digitalWrite(clock,HIGH);

}

/* THIS READS DATA FROM THE CONTROLLER */
void controllerRead() {
controller_data = 0;
digitalWrite(latch,LOW);
digitalWrite(clock,LOW);

digitalWrite(latch,HIGH);
delayMicroseconds(2);
digitalWrite(latch,LOW);

controller_data = digitalRead(datin);

for (int i = 1; i <= 7; i ++) {
digitalWrite(clock,HIGH);
delayMicroseconds(2);
//controller_data = controller_data << 1;
//controller_data = controller_data + digitalRead(datin) ;
controller_data += digitalRead(datin)*(1<<i);
delayMicroseconds(4);
digitalWrite(clock,LOW);
}

}

/* THE LED, SERVO, AND SERIAL MONITOR PROGRAM */
void loop() {
controllerRead();
//Serial.println(controller_data, BIN);
Serial.println(controller_data);

if (controller_data==B01111111){
  digitalWrite(ledpin, HIGH);   
  }
  
if (controller_data==B10111111){
  digitalWrite(ledpin, LOW);
 }

 delay(10);
} 
