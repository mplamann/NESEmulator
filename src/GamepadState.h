#pragma once
#import <allegro5/allegro.h>

class GamepadState
{
 public:
  GamepadState(void);
  ~GamepadState(void);
  bool initializeKeyboard(ALLEGRO_EVENT_QUEUE* event_queue);
};
