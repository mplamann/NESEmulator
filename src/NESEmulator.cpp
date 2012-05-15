#include <allegro5/allegro.h>

#include "CpuState.h"
#include "PpuState.h"
#include "MemoryState.h"
#include <time.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  CpuState* cpu = new CpuState();
  MemoryState* memory = new MemoryState();
  PpuState* ppu = new PpuState();
  
  ppu->setMemory(memory);
  cpu->setMemory(memory);

  ppu->initializeDisplay();
  
  memory->loadFileToRAM("../ROMs/SMB1.nes");
  cpu->doRESET();
  // NOTE: Execution starts at address pointed to by RESET vector
  bool done = false;
  while (!done)
    {
      cpu->RunInstruction();
      done = ppu->processEvents();
    }

  delete cpu;
  delete memory;
  delete ppu;
  return 0;
}

