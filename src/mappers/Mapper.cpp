#include "Mapper.h"
#include <string.h>
#include <iostream>
using namespace std;

Mapper::Mapper(char* file)
{
  nPrgBanks = file[4];
  nChrBanks = file[5];
  cout << "\n" << nPrgBanks << " PRG banks\n" << nChrBanks << " CHR banks\n";
  mapperNumber = ((file[6] & 0xF0) >> 4) + (file[7] & 0xF0);
  mirroring = file[6] & 0x9;
  
  // Initialize 2D arrays
  prgBanks = new char*[nPrgBanks];
  chrBanks = new char*[nChrBanks];
  for (int i = 0; i < nPrgBanks; i++)
    prgBanks[i] = new char[16*1024];
  for (int i = 0; i < (nChrBanks > 0 ? nChrBanks : 2); i++)
    chrBanks[i] = new char[8*1024];

  prgBank1Index = 0;
  prgBank2Index = 0;
  chrBank1Index = 0;
  chrBank2Index = 0;
  if (nPrgBanks > 1)
    prgBank2Index = 1;
  if (nChrBanks != 1)
    chrBank2Index = 1;

  int filePointer = 16;
  for (int i = 0; i < nPrgBanks; i++)
    {
      memcpy(prgBanks[i], file+filePointer, 16*1024);
      filePointer += 16*1024;
    }
  for (int i = 0; i < nChrBanks; i++)
    {
      memcpy(chrBanks[i], file+filePointer, 8*1024);
      filePointer += 8*1024;
    }
  return;
}

Mapper::~Mapper(void)
{
  for (int i = 0; i < nPrgBanks; i++)
    delete [] prgBanks[i];
  for (int i = 0; i < nChrBanks; i++)
    delete [] chrBanks[i];
  delete [] prgBanks;
  delete [] chrBanks;
  cout << "Mapper deallocated\n";
}

int Mapper::readByteFrom(int address)
{
  if (address < 0x8000)
    {
      cout << "Mapper.c does not know what to do with address " << address << ". Returning 0\n";
      return 0;
    }
  else if (nPrgBanks == 0)
    {
      return 0;
    }
  else if (address < 0xC000)
    {
      int adjustedAddress = address - 0x8000;
      return prgBanks[prgBank1Index][adjustedAddress];
    }
  else
    {
      int adjustedAddress = address - 0xC000;
      return prgBanks[prgBank2Index][adjustedAddress];
    }
}

void Mapper::writeByteTo(int address, int value)
{
  cout << "Mapper.cpp written to, can't do anything with this!\n";
}

int Mapper::ppuReadByteFrom(int address)
{
  char* bank = chrBanks[chrBank1Index];
  if (address >= 8*1024 && nChrBanks != 1)
    bank = chrBanks[chrBank2Index];
  return bank[address % (8*1024)];
}

void Mapper::ppuWriteByteTo(int address, int value)
{
  if (nChrBanks != 0)
    return;
  char* bank = chrBanks[chrBank1Index];
  if (address >= 8*1024 && nChrBanks != 1)
    bank = chrBanks[chrBank2Index];
  bank[address % (8*1024)] = (value & 0xFF);
}

size_t Mapper::stateSize()
{
  return 0;
}

char* Mapper::stateData()
{
  //return (char*)malloc(sizeof(char)*stateSize());
  return new char[stateSize()];
}

void Mapper::loadState(char* ignored)
{
}
