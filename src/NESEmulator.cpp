#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include <time.h>
#include <iostream>
using namespace std;

const int PPU_CYCLES_PER_SCANLINE = 1364;
const int CPU_CYCLES_PER_PPU_CYCLE = 12;

ALLEGRO_EVENT_QUEUE* event_queue;

bool setupAllegroEvents();
bool processEvents();

int main(int argc, char **argv)
{
  CpuState* cpu = new CpuState();
  MemoryState* memory = new MemoryState();
  PpuState* ppu = new PpuState();
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);

  if (!setupAllegroEvents())
    {
      delete cpu;
      delete memory;
      delete ppu;
      return -1;
    }
  ppu->initializeDisplay(event_queue);
  
  memory->loadFileToRAM("../ROMs/twosprites.nes");
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
	{
	  targetCpuCycle = cpu->getCycles() + PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
	  while (cpu->getCycles() < targetCpuCycle)
	    cpu->RunInstruction();
	  ppu->renderScanline(scanline);
	}
      ppu->endFrame(); // Flip back buffer to screen
      done = processEvents();
    }

  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);
  delete cpu;
  delete memory;
  delete ppu;
  return 0;
}

bool setupAllegroEvents()
{
  event_queue = al_create_event_queue();
  if (!event_queue)
    {
      al_show_native_message_box(NULL,"Critical Error!",NULL,"failed to create event queue.",NULL,NULL);
      return false;
    }
}

bool processEvents()
{
  ALLEGRO_EVENT event;
  if (al_get_next_event(event_queue, &event))
    {
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
	  return true;
	}
    }
}
