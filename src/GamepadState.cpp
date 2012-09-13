#include "GamepadState.h"
#include <allegro5/allegro.h>

GamepadState::GamepadState(void)
{
}

GamepadState::~GamepadState(void)
{
}

bool GamepadState::initializeKeyboard()
{
    if (!al_install_keyboard())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize keyboard!", NULL, NULL);
      return false;
    }
}
