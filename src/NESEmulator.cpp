//#include "CpuState.h"
#include "SDL.h"
#include "CpuV2.h"
#include "PpuState.h"
#include "MemoryState.h"
#include "GamepadState.h"
#include "ApuState.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <signal.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

//#define USE_AUDIO

const int FRAMERATE = 60;
const int TURBO_FRAMERATE = 600;
int fps_limit = FRAMERATE;

const int PPU_STARTUP_TIME = 27384;
const float CPU_CYCLES_PER_SCANLINE = 113.66666667;

//ALLEGRO_EVENT_QUEUE* event_queue;
//ALLEGRO_TIMER* timer;
CpuV2* cpu;
MemoryState* memory;
PpuState* ppu;
GamepadState* gamepad;
#ifdef USE_AUDIO
ApuState* apu;
#endif

char* savefile;
char* batteryfile;

bool usingArduino;

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
  savefile = new char[strlen(argv[1]) + 4];
  strcpy(savefile, argv[1]);
  strcat(savefile, ".sav");

  batteryfile = new char[strlen(argv[1]) + 4];
  strcpy(batteryfile, argv[1]);
  strcat(batteryfile, ".bat");

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

  if (!ppu->initializeDisplay())
    { cleanup(); return -1; }
  if (!gamepad->initializeKeyboard())
    { cleanup(); return -1; }
  if (!(usingArduino = gamepad->initializeArduino()))
    { cout << "No arduino.\n"; } // Then we don't use the arduino. Live with it.

#ifdef USE_AUDIO
  if (!apu->initializeAudio())
    { cleanup(); return -1; }
#endif

  memory->loadFileToRAM(argv[1]);
  memory->loadBattery(batteryfile);
  cpu->doRESET();
  cpu->S = 0xFD;
  cpu->cycles = 0; // Compensate for initial doRESET. This is just to make cycles line up with Nintendulator.
  
  SDL_Event event;
  bool need_redraw = false;
  Uint32 lastTicks = 0;
  
  while (!emulationDone)
    {
      if ((SDL_GetTicks() - lastTicks) > (1000.0 / fps_limit))
	{
	  renderFrame();
	  if (shouldSaveState)
	    saveState(savefile);
	  else if (shouldLoadState)
	    loadState(savefile);
	  shouldSaveState = shouldLoadState = false;
	  lastTicks = SDL_GetTicks();
	}
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      emulationDone = true;
	    }
	  if (event.type == SDL_KEYDOWN)
	    {
	      if (event.key.keysym.sym == SDLK_f)
		need_redraw = true;
	      if (event.key.keysym.sym == SDLK_b)
		{
		  cout << "BREAK!\n";
		}
	      if (event.key.keysym.sym == SDLK_t)
		{
		  shouldSaveState = true;
		}
	      if (event.key.keysym.sym == SDLK_y)
		{
		  shouldLoadState = true;
		}
	      if (event.key.keysym.sym == SDLK_SPACE)
		fps_limit = TURBO_FRAMERATE;
	      if (event.key.keysym.sym == SDLK_q)
		emulationDone = true;
	      gamepad->keyDown(event.key);
	    }
	  if (event.type == SDL_KEYUP)
	    {
	      if (event.key.keysym.sym == SDLK_SPACE)
		fps_limit = FRAMERATE;
	      gamepad->keyUp(event.key);
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
  
  cleanup();
  cout << "Goodbye.\n";
  return 0;
}

void renderFrame()
{
  game_time = SDL_GetTicks();
      
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

void cleanup()
{
  delete savefile;
  delete batteryfile;
  delete cpu;
  delete memory;
  delete ppu;
  delete gamepad;

#ifdef USE_AUDIO
  delete apu;
#endif
}

/*void saveState(char* filename)
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
}*/

void saveState(char* filename)
{
  ofstream savefile (filename, ios::out | ios::binary);
  memory->saveState(savefile);
  cpu->saveState(savefile);
  ppu->saveState(savefile);
#ifdef USE_AUDIO
  apu->saveState(savefile);
#endif
  savefile.close();
  cout << "State saved.\n";
}

void loadState(char* filename)
{
  ifstream savefile (filename, ios::in | ios::binary);
  memory->loadState(savefile);
  cpu->loadState(savefile);
  ppu->loadState(savefile);
#ifdef USE_AUDIO
  apu->loadState(savefile);
#endif
  savefile.close();
}
