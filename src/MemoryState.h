#pragma once

#include "Util.h"
#include "Mapper.h"
#define RAM_SIZE 0x800

class MemoryState
{
private:
  Mapper* mapper;
  char RAM[RAM_SIZE];

  // PPU Registers
  char PPUCTRL;
  char PPUMASK;
  char PPUSTATUS;
  char OAMADDR;
  char OAMDATA;
  char PPUSCROLL;
  char PPUADDR;
  char PPUDATA;
public:
  MemoryState(void);
  ~MemoryState(void);
  
  int readByteFrom(int address);
  void writeByteTo(int address, int value);
  
  void loadFileToRAM(char* filename);

  int ppuReadByteFrom(int address);
  void ppuWriteByteTo(int address, int value);
  
};

