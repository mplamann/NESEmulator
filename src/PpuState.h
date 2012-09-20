#pragma once

#include "Util.h"
#include "MemoryState.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

class PpuState
{
 private:
  MemoryState* memory;
  ALLEGRO_DISPLAY* display;
  int cycles;
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
