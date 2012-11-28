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
//#define USE_AUDIO

const int PPU_STARTUP_TIME = 27384;
const float CPU_CYCLES_PER_SCANLINE = 113.6666;

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
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);
  memory->setGamepad(gamepad);
  memory->setCpu(cpu);

#ifdef USE_AUDIO
  apu = new ApuState();
  memory->setApu(apu);
  apu->setMemory(memory);
  apu->setCpu(cpu);
#endif

  if (!setupAllegroEvents())
    { cleanup(); return -1; }
  if (!ppu->initializeDisplay(event_queue))
    { cleanup(); return -1; }
  if (!gamepad->initializeKeyboard(event_queue))
    { cleanup(); return -1; }
  if (!(usingArduino = gamepad->initializeArduino()))
    { cout << "No arduino.\n"; } // Then we don't use the arduino. Live with it.

#ifdef USE_AUDIO
  if (!apu->initializeAudio(event_queue))
    { cleanup(); return -1; }
#endif

#ifndef RUN_TEST
  //memory->loadFileToRAM("../ROMs/controller.nes");
  //memory->loadFileToRAM("../ROMs/background/background.nes");
  //memory->loadFileToRAM("../ROMs/Castlevania.nes");
  memory->loadFileToRAM("../ROMs/SMB1.nes");
  //memory->loadFileToRAM("../ROMs/instr_test-v3/official_only.nes");
  //memory->loadFileToRAM("../ROMs/pong1.nes");
  //memory->loadFileToRAM("../ROMs/scrolling/scrolling5.nes");
  //memory->loadFileToRAM("../ROMs/MegaMan.nes");
  //memory->loadFileToRAM("../ROMs/Galaga.nes");
  //memory->loadFileToRAM("../ROMs/Super Mario Bros. 3.nes");
  //memory->loadFileToRAM("../ROMs/ppu_vbl_nmi/ppu_vbl_nmi.nes");
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

  cpu->incrementCycles(-6); // Compensate for initial doRESET. This is just to make cycles line up with Nintendulator.
  
  double old_time = al_get_time();
  double game_time = 0.0;
  
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;
  int scanline = 241; // This is the scanline that Nintendulator starts on

  float targetCycle = 0;
  while (!done)
    {
      game_time = al_get_time();
      
      // Render one frame
      for (; scanline < 262; scanline++)
	{
	  //if (usingArduino)
	  //  gamepad->readFromArduino();

	  if (scanline == 241 && cpu->getCycles() > PPU_STARTUP_TIME)
	    {
	      memory->PPUSTATUS |= 0x80;  // Set VINT flag
	      if (memory->PPUCTRL & 0x80)
		cpu->doNMI();
	    }
	  if (scanline == 0)
	    ppu->startFrame();
	  if (scanline < 241)
	    {
	      ppu->renderScanline(scanline);
	    }
	  if (scanline == 240)
	    ppu->endFrame();

	  targetCycle += CPU_CYCLES_PER_SCANLINE;
	  while (cpu->getCycles() < targetCycle)
	    {
	      if (!cpu->RunInstruction())
		break; // This means that an unknown instruction was run.
#ifdef CPU_DEBUG
	      else
		cout << " SL: " << dec << scanline << hex << " \n";
	      // CYC is in PPU cycles - 3 per CPU cycle
#endif
	    }
	}
      scanline = 0; // Reset scanline. This comes at the end to not interfere with startup state.

      if (game_time - old_time >= 1.0)
	{
	  fps = frames_done / (game_time - old_time);
	  frames_done = 0;
	  old_time = game_time;
	  char windowTitle[50];
	  sprintf(windowTitle, "nesemulator - %.2f FPS", fps);
	  ppu->setDisplayTitle(windowTitle);
	}

      frames_done++;

      done = processEvents();
    }
  
  /*while (!done)
    {
    double game_time = al_get_time();

    //      if (usingArduino)
    //gamepad->readFromArduino();

    if (memory->PPUCTRL & 0x80)
    cpu->doNMI();
    // VBlank lasts 20 scanlines + 1 dummy scanline and then another at the end of the frame.
    // I will just put that last dummy scanline here
    int targetCpuCycle = cpu->getCycles() + 21*PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
    while (cpu->getCycles() < targetCpuCycle)
    {
    done = !cpu->RunInstruction();
    if (done)
    break;
    }

    // Render frame
    ppu->startFrame();
    for (int scanline = 0; scanline < 240; scanline++)
    {
    targetCpuCycle = cpu->getCycles() + PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
    while (cpu->getCycles() < targetCpuCycle)
    {
    done = !cpu->RunInstruction();
    if (done)
    break;
    }
    if (scanline >= 8 && scanline <= 232)
    ppu->renderScanline(scanline);
    }

    // "Render" dummy scanline
    targetCpuCycle = cpu->getCycles() + PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
    while (cpu->getCycles() < targetCpuCycle)
    {
    done = !cpu->RunInstruction();
    if (done)
    break;
    }
      
    // This also sets the VBlank flag
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
    }

    frames_done++;
    }*/

  cout << "Cleaning up...";
  if (event_queue != NULL)
    al_destroy_event_queue(event_queue);
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;
#ifdef USE_AUDIO
  delete apu;
#endif
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
#ifdef USE_AUDIO
      if (event.type == ALLEGRO_EVENT_AUDIO_STREAM_FRAGMENT)
	{
	  apu->audioStreamFragment();
	}
#endif
    }
  return false;
}

void cleanup()
{
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;

#ifdef USE_AUDIO
  delete apu;
#endif
}
