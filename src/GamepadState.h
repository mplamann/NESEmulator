#pragma once
#import <allegro5/allegro.h>

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

  void keyDown(ALLEGRO_EVENT event);
  void keyUp(ALLEGRO_EVENT event);

  void strobe();
  bool readPlayer1();
  bool readPlayer2();
 private:
  gamepad player1;
  gamepad player2;
  gamepad latchedP1;
  gamepad latchedP2;
  int p1Index;
  int p2Index;
  void setValueForKey(ALLEGRO_EVENT event, bool value);
  bool gamepadValueForIndex(gamepad pad, int index);
};
