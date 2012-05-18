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
  event_queue = al_create_event_queue();
  if (!event_queue)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to create event queue.",NULL,NULL);
      return false;
    }
  al_register_event_source(event_queue, al_get_display_event_source(display));
  return true;
}

bool PpuState::processEvents()
{
  // The return value of this function is whether we should stop running
  // Will return true if exit button is clicked
  
  ALLEGRO_EVENT event;
  if (al_get_next_event(event_queue, &event))
    {
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
	  return true;
	}
    }
  return false;
}

void PpuState::renderScanline(int scanline)
{
}

void PpuState::endFrame()
{
}

PpuState::PpuState()
{
  display = NULL;
}

PpuState::~PpuState()
{
  if (display != NULL)
    al_destroy_display(display);
  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);
}

void PpuState::setMemory(MemoryState* mem)
{
  memory = mem;
}

int PpuState::getCycles() {return cycles;}
MemoryState* PpuState::getMemory() {return memory;}
