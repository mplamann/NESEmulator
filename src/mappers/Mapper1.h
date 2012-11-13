#pragma once

#include "Mapper.h"
#include "Util.h"

class Mapper1 : public Mapper
{
 public:
  Mapper1(char* file);
  ~Mapper1(void);
  void Mapper2::writeByteTo(int address, int value);
}
