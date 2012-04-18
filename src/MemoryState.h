#pragma once

#include "Util.h"
#include "Mapper.h"
#define RAM_SIZE 0x800

class MemoryState
{
private:
  Mapper* mapper;
  char RAM[RAM_SIZE];
public:
  MemoryState(void);
  ~MemoryState(void);
  
  int readByteFrom(int address);
  void writeByteTo(int address, int value);
  
  void loadFileToRAM(char* filename);
};

