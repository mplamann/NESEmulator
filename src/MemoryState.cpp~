#include "MemoryState.h"
#include <iostream>
#include <fstream>
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
  ifstream file (filename, ios::in|ios::binary|ios::ate);
  if (file.is_open())
    {
      ifstream::pos_type size = file.tellg();
      if (size > RAM_SIZE)
	size = RAM_SIZE;
      cout << "File (" << filename << ") too large, truncating.";
      file.seekg(0, ios::beg);
      file.read((char*)RAM, size);
      file.close();
    }
}
