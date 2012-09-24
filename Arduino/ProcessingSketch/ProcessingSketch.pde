/*
Description:	Interfacing a NES controller with a PC with an Arduino.
Coded by:	Prodigity
Date:		1 December 2011
Revision:	V0.9 (beta)
*/

import processing.serial.*;
import java.awt.*;

Serial arduino;

Robot VKey;

PImage bgImage;

char recvout;
char prevout;
char deltaout;

void setup() {
  size(434,180);
  frameRate(30);
  println(Serial.list());
  arduino = new Serial(this, Serial.list()[0], 9600); // ATTENTION!!!
  bgImage = loadImage("NEScontroller.jpg");
  try
  {
    VKey = new Robot();
  }
  catch(AWTException a){}
  prevout = 0;
}

void draw() {
  if (bgImage != null) { background(bgImage); }
  fill(255, 255, 0);
  serialRead();
  deltaout = (char)((int)recvout ^ (int)prevout);
  emulateKeyboard();
  prevout = recvout;
}

void serialRead() {
  while (arduino.available() > 0) {
    recvout = arduino.readChar();
  }
}

void emulateKeyboard() {
  if ((deltaout & 1  ) == 1  ) { if ((recvout & 1  ) == 1  ) {VKey.keyPress(KeyEvent.VK_L);} else {VKey.keyRelease(KeyEvent.VK_L);}}
  if ((deltaout & 2  ) == 2  ) { if ((recvout & 2  ) == 2  ) {VKey.keyPress(KeyEvent.VK_K);} else {VKey.keyRelease(KeyEvent.VK_K);}}
  if ((deltaout & 4  ) == 4  ) { if ((recvout & 4  ) == 4  ) {VKey.keyPress(KeyEvent.VK_G);} else {VKey.keyRelease(KeyEvent.VK_G);}}
  if ((deltaout & 8  ) == 8  ) { if ((recvout & 8  ) == 8  ) {VKey.keyPress(KeyEvent.VK_H);} else {VKey.keyRelease(KeyEvent.VK_H);}}
  if ((deltaout & 16 ) == 16 ) { if ((recvout & 16 ) == 16 ) {VKey.keyPress(KeyEvent.VK_W);} else {VKey.keyRelease(KeyEvent.VK_W);}}
  if ((deltaout & 32 ) == 32 ) { if ((recvout & 32 ) == 32 ) {VKey.keyPress(KeyEvent.VK_S);} else {VKey.keyRelease(KeyEvent.VK_S);}}
  if ((deltaout & 64 ) == 64 ) { if ((recvout & 64 ) == 64 ) {VKey.keyPress(KeyEvent.VK_A);} else {VKey.keyRelease(KeyEvent.VK_A);}}
  if ((deltaout & 128) == 128) { if ((recvout & 128) == 128) {VKey.keyPress(KeyEvent.VK_D);} else {VKey.keyRelease(KeyEvent.VK_D);}}
}
