#include "Mapper.h"
#include <string.h>
#include <iostream>
using namespace std;

Mapper::Mapper(char* file)
{
  nPrgBanks = file[4];
  nChrBanks = file[5];
  mapperNumber = ((file[6] & 0xF0) >> 4) + (file[7] & 0xF0);

  // Initialize 2D arrays
  prgBanks = new char*[nPrgBanks];
  chrBanks = new char*[nChrBanks];
  for (int i = 0; i < nPrgBanks; i++)
    prgBanks[i] = new char[16*1024];
  for (int i = 0; i < nChrBanks; i++)
    chrBanks[i] = new char[8*1024];

  prgBank1Index = 0;
  prgBank2Index = 0;
  if (nPrgBanks > 1)
    prgBank2Index = 1;

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
  if (address < 0x8000)
    {
      cout << "Mapper.c does not know what to do with address " << address << ". Returning.\n";
    }
  else if (address < 0xC000)
    {
      int adjustedAddress = address - 0x8000;
      prgBanks[prgBank1Index][adjustedAddress] = value;
    }
  else
    {
      int adjustedAddress = address - 0xC000;
      prgBanks[prgBank1Index][adjustedAddress] = value;
    }
}

int Mapper::ppuReadByteFrom(int address)
{
  char* bank = chrBanks[0];
  if (address >= 8*1024 && nChrBanks > 1)
    bank = chrBanks[1];
  return bank[address % (8*1024)];
}

void Mapper::ppuWriteByteTo(int address, int value)
{
  char* bank = chrBanks[0];
  if (address >= 8*1024 && nChrBanks > 1)
    bank = chrBanks[1];
  bank[address % (8*1024)] = (value & 0xFF);
}
