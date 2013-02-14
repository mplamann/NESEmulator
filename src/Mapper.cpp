#include "Mapper.h"
#include <string.h>
#include <iostream>
using namespace std;

Mapper::Mapper(char* file, int _prgBankSize, int _chrBankSize)
{
  prgBankSize = _prgBankSize;
  chrBankSize = _chrBankSize;
  float prgBankMultiplier = 16.0 / prgBankSize;
  float chrBankMultiplier = 8.0 / chrBankSize;
  nPrgBanks = prgBankMultiplier*file[4];
  nChrBanks = chrBankMultiplier*file[5];
  cout << "\n0x" << nPrgBanks << " 0x" << prgBankSize << "KB PRG banks\n0x" << nChrBanks << " 0x" << chrBankSize << "KB CHR banks\n";
  mapperNumber = ((file[6] & 0xF0) >> 4) + (file[7] & 0xF0);
  mirroring = file[6] & 0x9;
  batteryBacked = file[6] & 0x02;
  prgRamEnabled = false;
  prgRamWritable = true;
  
  // Initialize 2D arrays
  prgBanks = new char*[nPrgBanks];
  chrBanks = new char*[nChrBanks];
  for (int i = 0; i < nPrgBanks; i++)
    prgBanks[i] = new char[prgBankSize*1024];
  for (int i = 0; i < (nChrBanks > 0 ? nChrBanks : chrBankMultiplier); i++)
    chrBanks[i] = new char[chrBankSize*1024];

  prgIndexes = new int[32/prgBankSize];
  chrIndexes = new int[8/chrBankSize];
  for (int i = 0; i < 32/prgBankSize; i++)
    prgIndexes[i] = 0;
  for (int i = 0; i < 8/chrBankSize; i++)
    chrIndexes[i] = 0;
  chrIndexes[1] = 1;
  if (nPrgBanks > 1)
    prgIndexes[1] = 1;

  int filePointer = 16;
  for (int i = 0; i < nPrgBanks; i++)
    {
      memcpy(prgBanks[i], file+filePointer, prgBankSize*1024);
      filePointer += prgBankSize*1024;
    }
  for (int i = 0; i < nChrBanks; i++)
    {
      memcpy(chrBanks[i], file+filePointer, chrBankSize*1024);
      filePointer += chrBankSize*1024;
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
  if (address < 0x6000)
    {
      //      cout << "Mapper.c does not know what to do with address " << address << ". Returning 0\n";
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
  address -= 0x8000;
  int prgBankIndex = (address / (1024*prgBankSize));
  int adjustedAddress = (address % (1024*prgBankSize));
  return prgBanks[prgIndexes[prgBankIndex]][adjustedAddress];
}

void Mapper::writeByteTo(int address, int value)
{
  if (address >= 0x6000 && address < 0x8000)
    {
      if (prgRamEnabled && prgRamWritable)
	{
	  prgRam[address-0x6000] = value;
	  return;
	}
      else
	{
	  return;
	}
    }
  cout << "Mapper.cpp written to, can't do anything with this!\n";
}

int Mapper::ppuReadByteFrom(int address)
{
  int chrBankIndex = (address / (1024*chrBankSize));
  int adjustedAddress = (address % (1024*chrBankSize));
  return chrBanks[chrIndexes[chrBankIndex]][adjustedAddress];
}

void Mapper::ppuWriteByteTo(int address, int value)
{
  if (nChrBanks != 0)
    return;
  int chrBankIndex = (address / (1024*chrBankSize));
  int adjustedAddress = (address % (1024*chrBankSize));
  chrBanks[chrIndexes[chrBankIndex]][adjustedAddress] = value & 0xFF;
}

size_t Mapper::stateSize()
{
  return 0;
}

char* Mapper::stateData()
{
  return new char[stateSize()];
}

void Mapper::loadState(char* ignored)
{
}

void Mapper::scanlineCounter()
{
}
