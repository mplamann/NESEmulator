#pragma once

#include "Util.h"
#include "MemoryState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const int scale = 3;

class PpuState
{
 private:
  int width, height;
  MemoryState* memory;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_DISPLAY* nametableDisplay;
  ALLEGRO_DISPLAY* paletteDisplay;
  ALLEGRO_COLOR blackColor;
  int cycles;
  ALLEGRO_VERTEX pointList[256*224];
  int vScroll; // Vertical scroll is preserved during the entire frame.
               // Keep a local copy so that changes to PPUSCROLLY doesn't affect mid-frame.

  ALLEGRO_VERTEX framePoints[256*scale*240];
  ALLEGRO_VERTEX* scanlinePoints;

  inline void incrementX();
  inline void incrementY();
  
 public:
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

  char* stateData(size_t* size);
  void loadState(char* state, size_t size);
};
