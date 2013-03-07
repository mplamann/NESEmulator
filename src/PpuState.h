#pragma once

#include "Util.h"
#include "MemoryState.h"
#include "SDL.h"
#include <fstream>
using namespace std;

const int scale = 3;
const int bpp = 32;

class PpuState
{
 private:
  int width, height;
  MemoryState* memory;
  SDL_Surface* display;
  SDL_Surface* backbuffer;
  int cycles;
  int vScroll; // Vertical scroll is preserved during the entire frame.
               // Keep a local copy so that changes to PPUSCROLLY doesn't affect mid-frame.

  bool backgroundPoints[256];
  bool alreadyDisabled[256];

  void recalculateTiles();
  
  inline void incrementX();
  inline void incrementY();
  inline void renderBackground(int scanline);
  inline void renderSprites(int scanline);
  
 public:
  bool needsRecalc;
  bool initializeDisplay();
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
