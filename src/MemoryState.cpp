#include "MemoryState.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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
  if (address >= RAM_SIZE || address < 0)
    {
      cout << "Invalid memory read at address " << address << endl;
    }
  return RAM[address];
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
  long size;
  size_t result;

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

  if (size > RAM_SIZE)
    {
      cout << "File larger than RAM, aborting.";
      fclose(fileStream);
      return;
    }

  result = fread (RAM,1,size,fileStream);
  if (result != size)
    {
      cout << "Reading error. ROM not loaded.";
      fclose(fileStream);
      return;
    }
  fclose(fileStream);
  return;
}
