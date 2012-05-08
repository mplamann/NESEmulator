#include "MemoryState.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Mapper0.h"
using namespace std;

MemoryState::MemoryState(void)
{
  for (int i = 0; i < RAM_SIZE; i++)
    RAM[i] = 0;
}

MemoryState::~MemoryState(void)
{
}

////////////////////////////////////////////////////////////////////////
// Read/Write Operations
////////////////////////////////////////////////////////////////////////

int MemoryState::readByteFrom(int address)
{
  if (address < 0x2000)
    {
      return (RAM[address % 0x800]) & 0xFF;
    }
  else if (address < 0x4000)
    {
      return -1; // return IORegisters[0x2000 + (address-0x2000)%8];
    }
  else
    {
      return (mapper->readByteFrom(address)) & 0xFF;
    }
}

void MemoryState::writeByteTo(int address, int value)
{
  if (address < 0x2000)
    RAM[address] = (value & 0xFF); // Each byte only holds 8 bits of data
  else if (address < 0x4000)
    return; // Unknown behavior here
  else
    {
      mapper->writeByteTo(address, value);
    }
}

int MemoryState::ppuReadByteFrom(int address)
{
  if (address < 0x2000)
    return (mapper->ppuReadByteFrom(address)) & 0xFF;
  else if (address < 0x2400)
    {
      return readFromNametable(0,address) & 0xFF;
    }
  else if (address < 0x2800)
    return readFromNametable(1,address) & 0xFF;
  else if (address < 0x2C00)
    return readFromNametable(2,address) & 0xFF;
  else if (address < 0x3000)
    return readFromNametable(3,address) & 0xFF;
  else if (address < 0x3F00)
    return ppuReadByteFrom(address - 0x1000) & 0xFF;
  else
    return palette[(address-0x3F20) % 0x20] & 0xFF; // Rest of RAM is just palette mirrored
}

void MemoryState::ppuWriteByteTo(int address, int value)
{
  if (address < 0x2000)
    mapper->ppuWriteByteTo(address, value & 0xFF);
  else if (address < 0x2400)
    writeToNametable(0,address,value & 0xFF);
  else if (address < 0x2800)
    writeToNametable(1,address,value & 0xFF);
  else if (address < 0x2C00)
    writeToNametable(2,address,value & 0xFF);
  else if (address < 0x3000)
    writeToNametable(3,address,value & 0xFF);
  else if (address < 0x3F00)
    ppuWriteByteTo(address - 0x1000, value & 0xFF);
  else
    palette[(address-0x3F20) % 0x20] = value & 0xFF; // Rest of RAM is just palette mirrored
}

int MemoryState::readFromNametable(int nametable, int address)
{
  int currentNametable = 1;
  if (mirroring == 0) // Horizontal Mirroring
    {
      if (nametable > 1)
	currentNametable = 2;
    }
  else if (mirroring == 1) // Vertical Mirroring
    {
      if (nametable == 1 || nametable == 3)
	currentNametable = 2;
    }
  else
    {
    } // TODO: Single-Screen Mirroring - needs mapper support
  int arrayAddress = (address - 0x2400) % 0x400;
  if (currentNametable == 1)
    return nametable1[arrayAddress];
  else
    return nametable2[arrayAddress];
}

void MemoryState::writeToNametable(int nametable, int address, int value)
{
  int currentNametable = 1;
  if (mirroring == 0) // Horizontal Mirroring
    {
      if (nametable > 1)
	currentNametable = 2;
    }
  else if (mirroring == 1) // Vertical Mirroring
    {
      if (nametable == 1 || nametable == 3)
	currentNametable = 2;
    }
  else
    {
    } // TODO: Single-Screen Mirroring - needs mapper support
  int arrayAddress = (address - 0x2400) % 0x400;
  if (currentNametable == 1)
    nametable1[arrayAddress] = value;
  else
    nametable2[arrayAddress] = value;
}

////////////////////////////////////////////////////////////////////////
// File IO
////////////////////////////////////////////////////////////////////////

void MemoryState::loadFileToRAM(char* filename)
{
  FILE* fileStream;
  size_t size;
  size_t result;
  char* file;

  fileStream = fopen(filename, "rb");
  if (fileStream == NULL)
    {
      cout << "Could not read file. ROM not loaded.";
      return;
    }

  // obtain file size
  fseek(fileStream, 0, SEEK_END);
  size = ftell(fileStream);
  rewind(fileStream);

  file = (char*)malloc(sizeof(char)*size);
  if (file == NULL)
    {
      cout << "Memory error. ROM not loaded.";
      fclose(fileStream);
      return;
    }

  result = fread (file,1,size,fileStream);
  if (result != size)
    {
      cout << "Reading error. ROM not loaded.";
      fclose(fileStream);
      return;
    }
  fclose(fileStream);

  bool isROM = (file[0] == 'N' && file[1] == 'E' && file[2] == 'S' && file[3] == 0x1A);
  if (!isROM)
    {
      cout << "This is not a NES rom.";
      return;
    }

  int mapperNumber = ((file[6] & 0xF0) >> 4) + (file[7] & 0xF0);

  switch (mapperNumber)
    {
    case 0:
      mapper = new Mapper0(file);
    }

  mirroring = file[6] & 0x9;
  
  return;
}
