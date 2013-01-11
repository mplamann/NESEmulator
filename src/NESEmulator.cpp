#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>


#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include "GamepadState.h"
#include "ApuState.h"
#include "apu_snapshot.h"
#include <time.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define USE_AUDIO

#define OSX
//#define IOS

const int FRAMERATE = 60;

const int PPU_STARTUP_TIME = 27384;
const float CPU_CYCLES_PER_SCANLINE = 113.66666667;

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
CpuState* cpu;
MemoryState* memory;
PpuState* ppu;
GamepadState* gamepad;
ApuState* apu;

bool usingArduino;

bool setupAllegroEvents();
bool processEvents();
void cleanup();
void renderFrame();
void saveState();
void loadState();

double fps = 0;
int frames_done = 0;
float targetCycle = 0;
double old_time = 0.0;
double game_time = 0.0;
int scanline = 241; // This is the scanline that Nintendulator starts on

bool shouldSaveState = false;
bool shouldLoadState = false;

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

  //memory->loadFileToRAM("../ROMs/controller.nes");
  //memory->loadFileToRAM("../ROMs/background/background.nes");
  //memory->loadFileToRAM("../ROMs/Castlevania.nes");
  //memory->loadFileToRAM("../ROMs/Super Mario Bros. (JU) [!].nes");
  //memory->loadFileToRAM("../ROMs/MapperTest/mapper2.nes");
  
  //memory->loadFileToRAM("../ROMs/square1/square1.nes");
  //memory->loadFileToRAM("../ROMs/cpu_timing_test.nes");
  //memory->loadFileToRAM("../ROMs/cpu_timing_test/cpu_timing_test.nes");
  //memory->loadFileToRAM("../ROMs/instr_test-v3/official_only.nes");
  //memory->loadFileToRAM("../ROMs/pong1.nes");
  //memory->loadFileToRAM("../ROMs/scrolling/scrolling5.nes");
  //memory->loadFileToRAM("../ROMs/MegaMan.nes");
  //memory->loadFileToRAM("../ROMs/Final Fantasy.nes");
  //memory->loadFileToRAM("../ROMs/Mega Man 2.nes");
  //memory->loadFileToRAM("../ROMs/Castlevania2.nes");
  //memory->loadFileToRAM("../ROMs/Metroid.nes");
  //memory->loadFileToRAM("../ROMs/Zelda.nes");
  //memory->loadFileToRAM("../ROMs/Pac-Man.nes");
  //memory->loadFileToRAM("../ROMs/Galaga.nes");
  memory->loadFileToRAM("../ROMs/Dragon Warrior 2.nes");
  //memory->loadFileToRAM("../ROMs/Excitebike.nes");
  //memory->loadFileToRAM("../ROMs/Galaxian.nes");
  //memory->loadFileToRAM("../ROMs/Super Mario Bros. 3.nes");
  //memory->loadFileToRAM("../ROMs/ppu_vbl_nmi/ppu_vbl_nmi.nes");
  cpu->doRESET();
  cpu->setS(0xFD);

  cpu->incrementCycles(-6); // Compensate for initial doRESET. This is just to make cycles line up with Nintendulator.
  
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;

  ALLEGRO_EVENT event;
  bool need_redraw = false;
  
  while (!done)
    {
      while (al_get_next_event(event_queue, &event))
	{
	  if (event.type == ALLEGRO_EVENT_TIMER)
	    {
	      need_redraw = true;
	    }
	  if (need_redraw && al_event_queue_is_empty(event_queue))
	    {
	      renderFrame();
	      need_redraw = false;
	      if (shouldSaveState)
		saveState();
	      else if (shouldLoadState)
		loadState();
	      shouldSaveState = shouldLoadState = false;
	    }
	  
	  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	    {
	      done = true;
	    }
	  if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
	      if (event.keyboard.keycode == ALLEGRO_KEY_B)
		{
		  cout << "BREAK!\n";
		}
	      if (event.keyboard.keycode == ALLEGRO_KEY_T)
		{
		  shouldSaveState = true;
		}
	      if (event.keyboard.keycode == ALLEGRO_KEY_Y)
		{
		  shouldLoadState = true;
		}
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
    }

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

void renderFrame()
{
  game_time = al_get_time();
      
  // Render one frame
  for (; scanline < 262; scanline++)
    {
      //if (usingArduino)
	//        gamepad->readFromArduino();

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

      cpu->RunForCycles(CPU_CYCLES_PER_SCANLINE, scanline);
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

  apu->finishFrame();
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
  cout << "Initializing FPS timer...";
  timer = al_create_timer(1.0 / FRAMERATE);
  if (!timer)
    {
      cout << "Error!\n";
      return false;
    }
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_start_timer(timer);
  return true;
}

void cleanup()
{
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;
  al_destroy_timer(timer);

#ifdef USE_AUDIO
  delete apu;
#endif
}

void saveState()
{
  cout << "Saving state...\n";
  
  size_t memorySize;
  size_t cpuSize;
  size_t ppuSize;

  char* memoryData;
  char* cpuData;
  char* ppuData;
  apu_snapshot_t* apuData = (apu_snapshot_t*)malloc(sizeof(apu_snapshot_t));

  memoryData = memory->stateData(&memorySize);
  cpuData = cpu->stateData(&cpuSize);
  ppuData = ppu->stateData(&ppuSize);
  apu->apu->save_snapshot(apuData);

  cout << "Header fields are " << sizeof(size_t) << " bytes.\n";
  
  cout << "MemoryState " << memorySize * sizeof(char) << " bytes.\n";
  cout << "CpuState    " << cpuSize * sizeof(char) << " bytes.\n";
  cout << "PpuState    " << ppuSize * sizeof(char) << " bytes.\n";

  FILE* fileStream = fopen("state.sav","wb");
  
  // Write header - memorySize,cpuSize,ppuSize
  size_t header[3];
  header[0] = memorySize;
  header[1] = cpuSize;
  header[2] = ppuSize;
  fwrite(header, sizeof(size_t), 3, fileStream);

  // Write state data
  fwrite(memoryData, sizeof(char), memorySize, fileStream);
  fwrite(cpuData, sizeof(char), cpuSize, fileStream);
  fwrite(ppuData, sizeof(char), ppuSize, fileStream);
  fwrite(apuData, sizeof(apu_snapshot_t), 1, fileStream);
  fclose(fileStream);

  free(memoryData);
  free(cpuData);
  free(ppuData);

  cout << "State saved.\n";
}

void loadState()
{
  cout << "Loading state...\n";

  size_t header[3];
  
  char* memoryData;
  char* cpuData;
  char* ppuData;
  apu_snapshot_t* apuData;

  FILE* fileStream = fopen("state.sav", "rb");
  fread(header, sizeof(size_t), 3, fileStream);

  memoryData = (char*)malloc(header[0]*sizeof(char));
  cpuData = (char*)malloc(header[1]*sizeof(char));
  ppuData = (char*)malloc(header[2]*sizeof(char));
  apuData = (apu_snapshot_t*)malloc(sizeof(apu_snapshot_t));
  
  fread(memoryData, sizeof(char), header[0], fileStream);
  fread(cpuData, sizeof(char), header[1], fileStream);
  fread(ppuData, sizeof(char), header[2], fileStream);
  fread(apuData, sizeof(apu_snapshot_t), 1, fileStream);
  
  fclose(fileStream);

  memory->loadState(memoryData, header[0]);
  cpu->loadState(cpuData, header[1]);
  ppu->loadState(ppuData, header[2]);
  apu->apu->load_snapshot(*apuData);

  free(memoryData);
  free(cpuData);
  free(ppuData);
}
