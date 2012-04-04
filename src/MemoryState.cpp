#include "MemoryState.h"
#include <iostream>
#include <fstream>
using namespace std;

MemoryState::MemoryState(void)
{
  for (int i = 0; i < RAM_SIZE; i++)
    RAM[i] = 0;
  RAM[0] = 0xA9; // LDA #23
  RAM[1] = 0x17;
  RAM[2] = 0x60; // ADC $0A00
  RAM[3] = 0x00;
  RAM[4] = 0x0A;
  RAM[5] = 0x29;
  RAM[6] = 0x08;
  RAM[0x0A00] = 0x10;
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
