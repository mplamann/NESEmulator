#include <allegro5/allegro.h>

#include "CpuState.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  CpuState* cpu = new CpuState();
  cpu->getMemory()->loadFileToRAM("../ROMs/SMB1.nes");
  cpu->doRESET();
  // NOTE: Execution starts at address pointed to by RESET vector
  while (true)
    {
      cpu->RunInstruction();
      cout << "\nPC: " << cpu->getPC();
      cout << "\nA: " << cpu->getA() << "\nX: " << cpu->getX() << "\nY: " << cpu->getY();
      char temp;
      cin >> temp;
    }
  return 0;
}

