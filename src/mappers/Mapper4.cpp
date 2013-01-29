#include "Mapper4.h"
#include <iostream>
using namespace std;

Mapper4::Mapper4(char* file) : Mapper(file)
{
  cout << "using mapper 4...";
  lastPpuAddr = -1;
  counterValue = 1;
  
  prgBank1Index = 0;
  prgBank2Index = nPrgBanks-1;
}

Mapper4::~Mapper4(void)
{
}

void Mapper4::writeByteTo(int address, int value)
{
  
}

void Mapper4::ppuWriteByteTo(int address, int value)
{
}

void Mapper4::scanlineCounter()
{
  counterValue--;
  
}

void Mapper4::updatePpuAddr(int address)
{
  if (lastPpuAddr != -1)
    {
      if (!(lastPpuAddr & 0x0100) && (address & 0x0100))
	{
	  // Rising edge of A12
	  scanlineCounter();
	}
    }
  lastPpuAddr = address;
}
