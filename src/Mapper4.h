#pragma once

#include "Mapper.h"
#include "Util.h"

class Mapper4 : public Mapper
{
public:
  Mapper4(char* file);
  ~Mapper4(void);
  int readByteFrom(int address);
  void writeByteTo(int address, int value);
  void ppuWriteByteTo(int address, int value);
  void updatePpuAddr(int address);

private:
  bool fireIRQs;
  int targetBank;
  int irqCounter;
  int irqLatch;
  int chrMode;
  int prgMode;

  void bankSwitch(int value);
  void scanlineCounter();
  int lastPpuAddr;
  int counterValue;
};

