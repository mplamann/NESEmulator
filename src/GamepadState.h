#pragma once
#include "SDL.h"
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
  bool initializeKeyboard();
  bool initializeArduino();

  void keyDown(SDL_KeyboardEvent event);
  void keyUp(SDL_KeyboardEvent event);

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
  void setValueForKey(SDL_KeyboardEvent event, bool value);
  bool gamepadValueForIndex(gamepad pad, int index);
  void setStateArduino(int arduinoState);
  Serial* serial;
};
