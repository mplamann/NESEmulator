#pragma once

#include "Util.h"
#include "MemoryState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <fstream>
using namespace std;

const int scale = 3;

class PpuState
{
 private:
  int width, height;
  MemoryState* memory;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_BITMAP* backbuffer;
  ALLEGRO_DISPLAY* nametableDisplay;
  ALLEGRO_DISPLAY* paletteDisplay;
  ALLEGRO_COLOR blackColor;
  int cycles;
  int vScroll; // Vertical scroll is preserved during the entire frame.
               // Keep a local copy so that changes to PPUSCROLLY doesn't affect mid-frame.

  ALLEGRO_VERTEX blankFrame[256*240];
  ALLEGRO_VERTEX framePoints[256*240];
  ALLEGRO_VERTEX* scanlinePoints;
  bool backgroundPoints[256];
  bool alreadyDisabled[256];

  void recalculateTiles();
  
  inline void incrementX();
  inline void incrementY();
  inline void renderBackground(int scanline);
  inline void renderSprites(int scanline);
  
 public:
  bool needsRecalc;
  bool initializeDisplay(ALLEGRO_EVENT_QUEUE* event_queue);
  void setDisplayTitle(const char* title);

  void startFrame();
  void renderScanline(int scanline);
  void endFrame();
    
  PpuState();
  ~PpuState();
  
  void setMemory(MemoryState* mem);
  MemoryState* getMemory();
  int getCycles();

  void saveState(ofstream& file);
  void loadState(ifstream& file);
};
