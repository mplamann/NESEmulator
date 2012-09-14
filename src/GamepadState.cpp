#include "GamepadState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
using namespace std;

GamepadState::GamepadState(void)
{
}

GamepadState::~GamepadState(void)
{
}

bool GamepadState::initializeKeyboard(ALLEGRO_EVENT_QUEUE* event_queue)
{
  cout << "Initializing gamepad...";
  if (!al_install_keyboard())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize keyboard!", NULL, NULL);
      return false;
    }
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  cout << "Done.\n";
  return true;
}
