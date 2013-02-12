#pragma once

#include "Mapper.h"

class Mapper7 : public Mapper
{
public:
  Mapper7(char* file);
  void writeByteTo(int address, int value);
};

