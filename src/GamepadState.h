#pragma once
#include <allegro5/allegro.h>
#include "serial.h"

struct gamepad {
  bool A;
  bool B;
  bool U;
  bool D;
  bool L;
  bool R;
  bool ST;
  bool SEL;
};

class GamepadState
{
 public:
  GamepadState(void);
  ~GamepadState(void);
  bool initializeKeyboard(ALLEGRO_EVENT_QUEUE* event_queue);
  bool initializeArduino();

  void keyDown(ALLEGRO_EVENT event);
  void keyUp(ALLEGRO_EVENT event);

  void strobe();
  bool readPlayer1();
  bool readPlayer2();

  void readFromArduino();
 private:
  gamepad player1;
  gamepad player2;
  gamepad latchedP1;
  gamepad latchedP2;
  int p1Index;
  int p2Index;
  void setValueForKey(ALLEGRO_EVENT event, bool value);
  bool gamepadValueForIndex(gamepad pad, int index);
  void setStateArduino(int arduinoState);
  Serial* serial;
};
