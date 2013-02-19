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
  irqCounter = 1;

  prgIndexes[2] = nPrgBanks - 2;
  prgIndexes[3] = nPrgBanks - 1;
}

Mapper4::~Mapper4(void)
{
}

void Mapper4::writeByteTo(int address, int value)
{
  //cout << "Mapper 4: 0x" << address << " = 0x" << value << "\n";
  value &= 0xFF;
  if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled)
	prgRam[address-0x6000] = value;
      return;
    }
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
      if (mirroring != 1-(value & 1))
	cout << "Mirroring: " << mirroring << " -> " << 1-(value & 1) << "\n";
      mirroring = 1-(value & 0x01);
      break;
    case 0xA001:
      prgRamEnabled = value & 0x80;
      prgRamWritable = value & 0x40;
      break;
    case 0xC000:
      irqLatch = value;
      if (!counter_latched)
	irqCounter = irqLatch;
      break;
    case 0xC001:
      irqCounter = irqLatch;
      counter_latched = false;
      break;
    case 0xE000:
      fireIRQs = false;
      if (!counter_latched)
	irqCounter = irqLatch;
      break;
    case 0xE001:
      fireIRQs = true;
      if (!counter_latched)
	irqCounter = irqLatch;
      break;
    }
}

void Mapper4::bankSwitch(int value)
{
  if (targetBank < 2)
    {
      chrIndexes[2*targetBank] = value;
      chrIndexes[2*targetBank+1] = value+1;
    }
  else if (targetBank < 6)
    chrIndexes[targetBank+2] = value;
  else
    prgIndexes[targetBank-6] = value;
}

////////////////////////////////////////////////////////////////////////////////
// CHR Bank switching                                                         //
////////////////////////////////////////////////////////////////////////////////

inline int Mapper4::chrBankNumber(int address)
{
  int bankNumber = address / 0x400;
  if (chrMode == CHR_2KB_LAST)
    {
      bankNumber += 4;
      bankNumber %= 8;
    }
  return bankNumber;
}

int Mapper4::ppuReadByteFrom(int address)
{
  int bankAddress = address % 0x400;
  return chrBanks[chrIndexes[chrBankNumber(address)]][bankAddress];
}

void Mapper4::ppuWriteByteTo(int address, int value)
{
  if (nChrBanks != 0)
    return;
  int bankAddress = address % 0x400;
  chrBanks[chrIndexes[chrBankNumber(address)]][bankAddress] = value;
}

////////////////////////////////////////////////////////////////////////////////
// PRG Bank switching                                                         //
////////////////////////////////////////////////////////////////////////////////

inline int Mapper4::prgBankNumber(int address)
{
  int bankNumber = (address-0x8000) / 0x2000;
  if (prgMode == PRG_SWITCH_MID && bankNumber == 0)
    bankNumber = 2;
  else if (prgMode == PRG_SWITCH_MID && bankNumber == 2)
    bankNumber = 0;
  return bankNumber;
}

int Mapper4::readByteFrom(int address)
{
  if (address < 0x6000)
    {
      return 0;
    }
  else if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled)
	return prgRam[address-0x6000];
      else
	return 0;
    }
  else if (nPrgBanks == 0)
    {
      return 0;
    }
  int bankAddress = address % 0x2000;
  int bank = prgBankNumber(address);
  return prgBanks[prgIndexes[bank]][bankAddress];
}

////////////////////////////////////////////////////////////////////////////////
// IRQ Counter                                                                //
////////////////////////////////////////////////////////////////////////////////

void Mapper4::scanlineCounter()
{
  counter_latched = true;
  if (irqCounter--)
    return;
  irqCounter = irqLatch;
  if (fireIRQs)
    {
      cpu->doIRQ();
      counter_latched = false;
    }
}
