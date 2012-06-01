#include <allegro5/allegro.h>

#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include <time.h>
#include <iostream>
using namespace std;

const int PPU_CYCLES_PER_SCANLINE = 1364;
const int CPU_CYCLES_PER_PPU_CYCLE = 12;

int main(int argc, char **argv)
{
  CpuState* cpu = new CpuState();
  MemoryState* memory = new MemoryState();
  PpuState* ppu = new PpuState();
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);

  ppu->initializeDisplay();
  
  //  memory->loadFileToRAM("../ROMs/sprites.nes");
  memory->loadFileToRAM("../ROMs/SMB1.nes");
  cpu->doRESET();
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;
  while (!done)
    {
      cpu->doNMI();
      // VBlank lasts 20 scanlines + 1 dummy scanline and then another at the end of the frame.
      // I will just put that last dummy scanline here
      int targetCpuCycle = cpu->getCycles() + 22*PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
      while (cpu->getCycles() < targetCpuCycle)
	cpu->RunInstruction();

      ppu->startFrame();
      for (int scanline = 0; scanline < 240; scanline++)
	{
	  targetCpuCycle = cpu->getCycles() + PPU_CYCLES_PER_SCANLINE/CPU_CYCLES_PER_PPU_CYCLE;
	  while (cpu->getCycles() < targetCpuCycle)
	    cpu->RunInstruction();
	  ppu->renderScanline(scanline);
	}
      ppu->endFrame(); // Flip back buffer to screen
      done = ppu->processEvents();
    }

  delete cpu;
  delete memory;
  delete ppu;
  return 0;
}

