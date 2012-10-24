#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>

#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include "GamepadState.h"
#include "ApuState.h"
#include <time.h>
#include <iostream>
#include <iomanip>
using namespace std;

//#define RUN_TEST

const int PPU_CYCLES_PER_SCANLINE = 1364;
const int CPU_CYCLES_PER_PPU_CYCLE = 12;

ALLEGRO_EVENT_QUEUE* event_queue;
CpuState* cpu;
MemoryState* memory;
PpuState* ppu;
GamepadState* gamepad;
ApuState* apu;

bool usingArduino;

bool setupAllegroEvents();
bool processEvents();
void cleanup();

double fps = 0;
int frames_done = 0;

int main(int argc, char **argv)
{
  cout << hex << uppercase;
  cpu = new CpuState();
  memory = new MemoryState();
  ppu = new PpuState();
  gamepad = new GamepadState();
  apu = new ApuState();
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);
  memory->setGamepad(gamepad);

  if (!setupAllegroEvents())
    { cleanup(); return -1; }
  if (!ppu->initializeDisplay(event_queue))
    { cleanup(); return -1; }
  if (!gamepad->initializeKeyboard(event_queue))
    { cleanup(); return -1; }
  if ((usingArduino = gamepad->initializeArduino()))
    {} // Then we don't use the arduino. Live with it.
  if (!apu->initializeAudio(event_queue))
    { cleanup(); return -1; }

#ifndef RUN_TEST
  //memory->loadFileToRAM("../ROMs/controller.nes");
  //memory->loadFileToRAM("../ROMs/background/background.nes");
  //memory->loadFileToRAM("../ROMs/Castlevania.nes");
  //memory->loadFileToRAM("../ROMs/SMB1.nes");
  //memory->loadFileToRAM("../ROMs/instr_test-v3/official_only.nes");
  //memory->loadFileToRAM("../ROMs/pong1.nes");
  memory->loadFileToRAM("../ROMs/scrolling/scrolling5.nes");
  //memory->loadFileToRAM("../ROMs/Mega Man (USA).nes");
  cpu->doRESET();
  #endif

#ifdef RUN_TEST
  memory->loadFileToRAM("../ROMs/nestest.nes");
  cout << "ROM Loaded\n";
  cpu->setPC(0xC000);
  cpu->setS(0xFD);

  while (true)
  cpu->RunInstruction();
#endif
  
  double old_time = al_get_time();
  
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;
  while (!done)
    {
      double game_time = al_get_time();

      if (usingArduino)
	gamepad->readFromArduino();

      if (memory->PPUCTRL & 0x80)
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

      if (game_time - old_time >= 1.0)
	{
	  fps = frames_done / (game_time - old_time);
	  frames_done = 0;
	  old_time = game_time;
	  char windowTitle[50];
	  sprintf(windowTitle, "nesemulator - %.2f FPS", fps);
	  ppu->setDisplayTitle(windowTitle);
	  //cout << fps << " FPS\n";
	}

      frames_done++;
    }

  cout << "Cleaning up...";
  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;
  delete apu;
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
      if (event.type == ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT)
	{
	  apu->audioStreamFragment();
	}
    }
  return false;
}

void cleanup()
{
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;
  delete apu;
}
