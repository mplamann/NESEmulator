#pragma once

#include "Mapper.h"
#include "Util.h"

class Mapper2 : public Mapper
{
public:
  Mapper2(char* file);
  ~Mapper2(void);
  void writeByteTo(int address, int value);
  void ppuWriteByteTo(int address, int value);
};
