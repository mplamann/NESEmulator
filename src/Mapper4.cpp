#include "Mapper4.h"
#include <iostream>
using namespace std;

const int CHR_2KB_FIRST = 0;
const int CHR_2KB_LAST  = 1;
const int PRG_SWITCH_FIRST = 0;
const int PRG_SWITCH_MID   = 1;

Mapper4::Mapper4(char* file) : Mapper(file, 8, 1)
{
  cout << "using mapper 4...";
  lastPpuAddr = -1;
  counterValue = 1;
  
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
      targetBank = value & 0x7;
      chrMode = (value & 0x80) >> 7;
      prgMode = (value & 0x40) >> 6;
      break;
    case 0x8001:
      bankSwitch(value);
      break;
    case 0xA000:
      mirroring = value & 0x01;
      break;
    case 0xA001:
      prgRamEnabled = value & 0x80;
      prgRamWritable = value & 0x40;
      break;
    case 0xC000:
      irqLatch = value;
      break;
    case 0xC001:
      irqCounter = irqLatch;
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

void Mapper4::bankSwitch(int value)
{
  if (targetBank < 6)
    chrIndexes[targetBank] = value;
  else
    prgIndexes[targetBank-6] = value;
}

int Mapper4::readByteFrom(int address)
{
  if (chrMode == CHR_2KB_FIRST)
    {
      
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
