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

int MemoryState::readByteFrom(int address)
{
  if (address < 0x2000)
    {
      return RAM[address % 0x800];
    }
  else if (address < 0x4000)
    {
      return -1; // return IORegisters[0x2000 + (address-0x2000)%8];
    }
  else
    {
      return mapper->readByteFrom(address);
    }
}

void MemoryState::writeByteTo(int address, int value)
{
  if (address >= RAM_SIZE || address < 0)
    {
      cout << "Invalid memory write at address " << address << endl;
    }
  RAM[address] = (value & 0xFF); // Each byte only holds 8 bits of data
}

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
  
  return;
}
