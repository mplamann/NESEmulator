#include "Mapper1.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
using namespace std;

Mapper1::Mapper1(char* file) : Mapper(file)
{
  cout << "using mapper 1...";
  shiftIndex = 0;
  shiftRegister = 0;
  prgBankMode = PRG_SWITCH_FIRST_16;
  chrBankMode = CHR_SWITCH_2x4;
  prgBankIndex = 0;
  updatePRGIndexes();
}

Mapper1::~Mapper1(void)
{
}

void Mapper1::writeByteTo(int address, int value)
{
#ifdef MAPPER_DEBUG
  if (address > 0x8000)
    cout << "Mapper \"" << address << "\" = " << value << "\n";
#endif
  if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled)
	{
	  prgRam[address-0x6000] = value;
	  return;
	}
      else
	{
	  return;
	}
    }
  shiftRegister |= (value & 0x01) << (shiftIndex++);

  if (value & 0x80)
    {
      shiftIndex = 0;
      shiftRegister = 0;
      prgBankMode = PRG_SWITCH_FIRST_16;
      return;
    }
  
  if (shiftIndex >= 5)
    {
#ifdef MAPPER_DEBUG
      cout << "Shift full...";
#endif
      switch (address & 0xF000)
	{
	case 0x8000:
	case 0x9000:
	  writeControl(shiftRegister);
	  break;
	case 0xA000:
	case 0xB000:
	  writeCHR0(shiftRegister);
	  break;
	case 0xC000:
	case 0xD000:
	  writeCHR1(shiftRegister);
	  break;
	case 0xE000:
	case 0xF000:
	  writePRG(shiftRegister);
	  break;
	default:
	  cout << "Unknown thingy: " << address << "\n";
	}
      shiftRegister = 0;
      shiftIndex = 0;
#ifdef MAPPER_DEBUG
      cout << "Emptied.\n";
#endif
    }
}

void Mapper1::writeControl(int value)
{
#ifdef MAPPER_DEBUG
  cout << "Control set to " << value << "\n";
#endif
  switch (value & 0x3)
    {
    case 0:
      mirroring = MIRRORING_LOWER_BANK;
      break;
    case 1:
      mirroring = MIRRORING_UPPER_BANK;
      break;
    case 2:
      mirroring = MIRRORING_VERTICAL;
      break;
    case 3:
      mirroring = MIRRORING_HORIZONTAL;
      break;
    }
  switch ((value & 0xC)>>2)
    {
    case 0:
    case 1:
      prgBankMode = PRG_SWITCH_32;
      break;
    case 2:
      prgBankMode = PRG_SWITCH_LAST_16;
      break;
    case 3:
      prgBankMode = PRG_SWITCH_FIRST_16;
      break;
    }
  if (value & 0x10)
    chrBankMode = CHR_SWITCH_2x4;
  else
    chrBankMode = CHR_SWITCH_8;
  updatePRGIndexes();
}

void Mapper1::writeCHR0(int value)
{
  if (nChrBanks == 0)
    return;
#ifdef MAPPER_DEBUG
  cout << "Bankswitch CHR0 to bank " << value << "\n";
#endif
  chrIndexes[0] = value;
  if (chrBankMode == CHR_SWITCH_8)
    {
      value &= 0x1E;
      chrIndexes[1] = value+1;
    }
}

void Mapper1::writeCHR1(int value)
{
  if (nChrBanks == 0)
    return;
#ifdef MAPPER_DEBUG
  cout << "Bankswitch CHR1 to bank " << value << "\n";
#endif
  chrIndexes[1] = value;
}

void Mapper1::writePRG(int value)
{
#ifdef MAPPER_DEBUG
  cout << "Bankswitch PRG to bank " << value << "\n";
#endif
  prgRamEnabled = !(value & 0x10);
  prgBankIndex = (value & 0xF);
  updatePRGIndexes();
}

void Mapper1::updatePRGIndexes()
{
  if (prgBankMode == PRG_SWITCH_32)
    {
      prgIndexes[0] = prgBankIndex & 0xE;
      prgIndexes[1] = prgIndexes[0]+1;
    }
  else if (prgBankMode == PRG_SWITCH_LAST_16)
    {
      prgIndexes[0] = 0;
      prgIndexes[1] = prgBankIndex;
    }
  else if (prgBankMode == PRG_SWITCH_FIRST_16)
    {
      prgIndexes[0] = prgBankIndex;
      prgIndexes[1] = nPrgBanks-1;
    }
}

size_t Mapper1::stateSize()
{
  int sPrgRam = sizeof(int)*0x2000;
  int sIndexes = sizeof(int)*(16/prgBankSize + 8/chrBankSize);
  int sOtherInts = sizeof(int)*5;
  return sPrgRam + sOtherInts + sIndexes + sizeof(bool);
}

char* Mapper1::stateData()
{
  char* buffer = (char*)malloc(sizeof(char)*stateSize());
  int bufferIndex = 0;
  memcpy(buffer+bufferIndex, prgRam, sizeof(int)*0x2000);
  bufferIndex+=sizeof(int)*0x2000;
  memcpy(buffer+bufferIndex, &prgRamEnabled, sizeof(bool));
  bufferIndex+=sizeof(bool);
  int otherValues[5] = {prgBankIndex, shiftRegister, shiftIndex, prgBankMode, chrBankMode};
  memcpy(buffer+bufferIndex, otherValues, 5*sizeof(int));
  bufferIndex+=sizeof(int)*5;
  memcpy(buffer+bufferIndex, prgIndexes, 16/prgBankSize*sizeof(int));
  bufferIndex += sizeof(int)*16/prgBankSize;
  memcpy(buffer+bufferIndex, chrIndexes, 8/chrBankSize*sizeof(int));
  return buffer;
}

void Mapper1::loadState(char* buffer)
{
  int bufferIndex = 0;
  memcpy(prgRam, buffer, sizeof(int)*0x2000);
  bufferIndex+=sizeof(int)*0x2000;
  memcpy(&prgRamEnabled, buffer+bufferIndex, sizeof(bool));
  bufferIndex+=sizeof(bool);
  int otherValues[5];
  memcpy(otherValues, buffer+bufferIndex, 5*sizeof(int));
  bufferIndex += 5*sizeof(int);
  memcpy(prgIndexes, buffer+bufferIndex, 16/prgBankSize*sizeof(int));
  bufferIndex += 16/prgBankSize*sizeof(int);
  memcpy(chrIndexes, buffer+bufferIndex, 8/chrBankSize*sizeof(int));
  prgBankIndex = otherValues[0];
  shiftRegister = otherValues[1];
  shiftIndex = otherValues[2];
  prgBankMode = otherValues[3];
  chrBankMode = otherValues[4];
}

void Mapper1::saveBattery(char* filename)
{
  FILE* fileStream = fopen(filename, "wb");
  fwrite(prgRam, sizeof(int), 0x2000, fileStream);
  fclose(fileStream);
}

void Mapper1::loadBattery(char* filename)
{
  FILE* fileStream = fopen(filename, "rb");
  if (fileStream != NULL)
    {
      fread(prgRam, sizeof(int), 0x2000, fileStream);
      fclose(fileStream);
    }
}
