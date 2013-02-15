#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>


//#include "CpuState.h"
#include "CpuV2.h"
#include "PpuState.h"
#include "MemoryState.h"
#include "GamepadState.h"
#include "ApuState.h"
#include "apu_snapshot.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <signal.h>
#include <stdlib.h>
using namespace std;

//#define USE_AUDIO

const int FRAMERATE = 60;
const int TURBO_FRAMERATE = 600;

const int PPU_STARTUP_TIME = 27384;
const float CPU_CYCLES_PER_SCANLINE = 113.66666667;

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
CpuV2* cpu;
MemoryState* memory;
PpuState* ppu;
GamepadState* gamepad;
#ifdef USE_AUDIO
ApuState* apu;
#endif

bool usingArduino;

bool setupAllegroEvents();
bool processEvents();
void cleanup();
void renderFrame();
void saveState(char* filename);
void loadState(char* filename);

double fps = 0;
int frames_done = 0;
float targetCycle = 0;
double old_time = 0.0;
double game_time = 0.0;
int scanline = 241; // This is the scanline that Nintendulator starts on

bool shouldSaveState = false;
bool shouldLoadState = false;

bool emulationDone = false;
void mark_done(int) { cout << "\n"; emulationDone = true; }

int main(int argc, char** argv)
{
  if (argc != 2)
    {
      cout << "Usage:\n" <<
              "  nesemulator rom_file\n";
      return 1;
    }
  char* savefile = new char[strlen(argv[1]) + 4];
  strcpy(savefile, argv[1]);
  strcat(savefile, ".sav");

  char* batteryfile = new char[strlen(argv[1]) + 4];
  strcpy(batteryfile, argv[1]);
  strcat(batteryfile, ".bat");

  // Trap Ctrl-C so that we can exit gracefully
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = mark_done;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  //sigaction(SIGINT, &sigIntHandler, NULL);

  cout << hex << uppercase;
  cpu = new CpuV2();
  memory = new MemoryState();
  ppu = new PpuState();
  gamepad = new GamepadState();

  ppu->setMemory(memory);
  cpu->memory = memory;
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

  memory->loadFileToRAM(argv[1]);
  memory->loadBattery(batteryfile);
  cpu->doRESET();
  cpu->S = 0xFD;
  cpu->cycles = 0; // Compensate for initial doRESET. This is just to make cycles line up with Nintendulator.
  
  ALLEGRO_EVENT event;
  bool need_redraw = false;
  
  while (!emulationDone)
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
		saveState(savefile);
	      else if (shouldLoadState)
		loadState(savefile);
	      shouldSaveState = shouldLoadState = false;
	    }
	  
	  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	    {
	      emulationDone = true;
	    }
	  if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
	      if (event.keyboard.keycode == ALLEGRO_KEY_F)
		need_redraw = true;
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
	      if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
		al_set_timer_speed(timer, 1.0/TURBO_FRAMERATE);
	      gamepad->keyDown(event);
	    }
	  if (event.type == ALLEGRO_EVENT_KEY_UP)
	    {
	      if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
		al_set_timer_speed(timer, 1.0/FRAMERATE);
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

  memory->saveBattery(batteryfile);
  
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
      
  for (; scanline < 262; scanline++)
    {
      //if (usingArduino)
	//        gamepad->readFromArduino();

      if (scanline == 241 && cpu->cycles > PPU_STARTUP_TIME)
	{
	  memory->PPUSTATUS |= 0x80;  // Set VINT flag
	  if (memory->PPUCTRL & 0x80)
	    {
	      cpu->doNMI();
	    }
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

#ifdef USE_AUDIO
  apu->finishFrame();
#endif
}

bool setupAllegroEvents()
{
  cout << "Initializing Allegro...";
  if (!al_init())
    {
      cout << "Error! Failed to initialize Allegro.\n";
      return false;
    }
  cout << "Done.\n";
  cout << "Initializing event queue...";
  event_queue = al_create_event_queue();
  if (!event_queue)
    {
      cout << "Error! Failed to create event queue.\n";
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

void saveState(char* filename)
{
  cout << "Saving state...\n";
  
  size_t memorySize;
  size_t cpuSize;
  size_t ppuSize;

  char* memoryData;
  char* cpuData;
  char* ppuData;
#ifdef USE_AUDIO
  apu_snapshot_t* apuData = (apu_snapshot_t*)malloc(sizeof(apu_snapshot_t));
#endif

  memoryData = memory->stateData(&memorySize);
  cpuData = cpu->stateData(&cpuSize);
  ppuData = ppu->stateData(&ppuSize);
#ifdef USE_AUDIO
  apu->apu->save_snapshot(apuData);
#endif

  cout << "Header fields are " << sizeof(size_t) << " bytes.\n";
  
  cout << "MemoryState " << memorySize * sizeof(char) << " bytes.\n";
  cout << "CpuState    " << cpuSize * sizeof(char) << " bytes.\n";
  cout << "PpuState    " << ppuSize * sizeof(char) << " bytes.\n";

  FILE* fileStream = fopen(filename,"wb");
  
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
#ifdef USE_AUDIO
  fwrite(apuData, sizeof(apu_snapshot_t), 1, fileStream);
#endif
  fclose(fileStream);

  free(memoryData);
  free(cpuData);
  free(ppuData);

  cout << "State saved.\n";
}

void loadState(char* filename)
{
  cout << "Loading state...\n";

  size_t header[3];
  
  char* memoryData;
  char* cpuData;
  char* ppuData;
#ifdef USE_AUDIO
  apu_snapshot_t* apuData;
#endif

  FILE* fileStream = fopen(filename, "rb");
  fread(header, sizeof(size_t), 3, fileStream);

  memoryData = (char*)malloc(header[0]*sizeof(char));
  cpuData = (char*)malloc(header[1]*sizeof(char));
  ppuData = (char*)malloc(header[2]*sizeof(char));
#ifdef USE_AUDIO
  apuData = (apu_snapshot_t*)malloc(sizeof(apu_snapshot_t));
#endif
  
  fread(memoryData, sizeof(char), header[0], fileStream);
  fread(cpuData, sizeof(char), header[1], fileStream);
  fread(ppuData, sizeof(char), header[2], fileStream);
#ifdef USE_AUDIO
  fread(apuData, sizeof(apu_snapshot_t), 1, fileStream);
#endif
  
  fclose(fileStream);

  memory->loadState(memoryData, header[0]);
  cpu->loadState(cpuData, header[1]);
  ppu->loadState(ppuData, header[2]);
#ifdef USE_AUDIO
  apu->apu->load_snapshot(*apuData);
#endif

  free(memoryData);
  free(cpuData);
  free(ppuData);
}
