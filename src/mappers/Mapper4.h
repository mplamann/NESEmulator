#pragma once

#include "Mapper.h"
#include "Util.h"

class Mapper4 : public Mapper
{
public:
  Mapper4(char* file);
  ~Mapper4(void);
  void writeByteTo(int address, int value);
  void ppuWriteByteTo(int address, int value);
  void updateppuAddr(int address);

private:
  bool fireIRQs;
  int command;
  int irqCounter;
  int irqLatch;

  void runCommand(int value);
  void scanlineCounter();
  int lastPpuAddr;
  int counterValue;
};

