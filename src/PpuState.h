#pragma once

#include "Util.h"
#include "MemoryState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

class PpuState
{
 private:
  int width, height, scale;
  MemoryState* memory;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_DISPLAY* nametableDisplay;
  ALLEGRO_DISPLAY* paletteDisplay;
  int cycles;
  ALLEGRO_VERTEX pointList[256*224];
  int vScroll; // Vertical scroll is preserved during the entire frame.
               // Keep a local copy so that changes to PPUSCROLLY doesn't affect mid-frame.
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
};
