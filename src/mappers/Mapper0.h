#pragma once

#include "Mapper.h"
#include "Util.h"

class Mapper0 : public Mapper
{
  char prgROM[16*KB];
  char chrROM[8*KB];
 public:
  int readByteFrom(int address);
  int writeByteTo(int address);
  void loadFromFile(char* file);
}
