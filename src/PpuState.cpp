#import "PpuState.h"

bool PpuState::initializeDisplay()
{
  int height = 640;
  int width = 480;
  display = NULL;
  if (!al_init())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize allegro!", NULL, NULL);
      return false;
    }
  display = al_create_display(width, height);
  if (!display)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to initialize display!", NULL,NULL);
      return false;
    }
  return true;
}

PpuState::~PpuState()
{
  al_destroy_display(display);
}
