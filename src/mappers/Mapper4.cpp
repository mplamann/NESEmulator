#include "Mapper4.h"
#include <iostream>
using namespace std;

Mapper4::Mapper4(char* file) : Mapper(file, 8)
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
  value &= 0xFF;
  switch (address)
    {
    case 0x8000:
      command = value;
      break;
    case 0x8001:
      runCommand(value);
      break;
    case 0xA000:
      mirroring = value & 0x01;
      break;
    case 0xA001:
      prgRamEnabled = value & 0x80;
      break;
    case 0xC000:
      irqCounter = value;
      break;
    case 0xC001:
      irqLatch = value;
      break;
    case 0xE000:
      fireIRQs = false;
      irqCounter = irqLatch;
      break;
    case 0xE001:
      fireIRQs = true;
      break;
    }
}

void Mapper4::runCommand(int value)
{
  switch (command & 0x7)
    {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    }
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
