#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include "GamepadState.h"
#include <time.h>
#include <iostream>
using namespace std;

const int PPU_CYCLES_PER_SCANLINE = 1364;
const int CPU_CYCLES_PER_PPU_CYCLE = 12;

ALLEGRO_EVENT_QUEUE* event_queue;
CpuState* cpu;
MemoryState* memory;
PpuState* ppu;
GamepadState* gamepad;

bool setupAllegroEvents();
bool processEvents();

int main(int argc, char **argv)
{
  cpu = new CpuState();
  memory = new MemoryState();
  ppu = new PpuState();
  gamepad = new GamepadState();
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);
  memory->setGamepad(gamepad);

  if (!setupAllegroEvents())
    {
      delete cpu;
      delete memory;
      delete ppu;
      return -1;
    }
  ppu->initializeDisplay(event_queue);
  gamepad->initializeKeyboard(event_queue);
  
  memory->loadFileToRAM("../ROMs/controller.nes");
  cout << "ROM Loaded\n";
  cpu->doRESET();
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;
  while (!done)
    {
      cpu->doNMI();
      // VBlank lasts 20 scanlines + 1 dummy scanline and then another at the end of the frame.
      // I will just put that last dummy scanline here
      int targetCpuCycle = cpu->getCycles() + 22*PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
      while (cpu->getCycles() < targetCpuCycle)
	cpu->RunInstruction();

      ppu->startFrame();
      for (int scanline = 0; scanline < 240; scanline++)
	//for (int scanline = 8; scanline < 232; scanline++)
	{
	  targetCpuCycle = cpu->getCycles() + PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
	  while (cpu->getCycles() < targetCpuCycle)
	    cpu->RunInstruction();
	  if (scanline >= 8 && scanline <= 232)
	    ppu->renderScanline(scanline);
	}
      ppu->endFrame(); // Flip back buffer to screen
      done = processEvents();
    }

  cout << "Cleaning up...";
  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);
  delete cpu;
  delete memory;
  delete ppu;
  cout << "Goodbye.\n";
  return 0;
}

bool setupAllegroEvents()
{
  cout << "Initializing Allegro...";
  if (!al_init())
    {
      al_show_native_message_box(NULL, "Critical Error!", NULL, "failed to initialize allegro!", NULL, NULL);
      cout << "Error!\n";
      return false;
    }
  cout << "Done.\n";
  cout << "Initializing event queue...";
  event_queue = al_create_event_queue();
  if (!event_queue)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to create event queue.",NULL,NULL);
      cout << "Error!\n";
      return false;
    }
  cout << "Done.\n";
  return true;
}

bool processEvents()
{
  ALLEGRO_EVENT event;
  while (al_get_next_event(event_queue, &event))
    {
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
	  return true;
	}
      if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
	  gamepad->keyDown(event);
	}
      if (event.type == ALLEGRO_EVENT_KEY_UP)
	{
	  gamepad->keyUp(event);
	}
    }
  return false;
}
