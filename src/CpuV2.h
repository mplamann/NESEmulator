#pragma once

#include "MemoryState.h"

class CpuV2
{
private:
  float accumulator;
public:
  int A;
  int X;
  int Y;
  int S;
  int PC;

  bool N,Z,C,I,D,V,B; // Processor status flags

  long cycles;
  int total_cycles;
  int cycles_remain;
  MemoryState* memory;
  
  CpuV2(void);
  ~CpuV2(void);
  bool RunInstruction();
  void RunForCycles(float cycle_cout, int scanline);

  char* stateData(size_t* size);
  void loadState(char* size, size_t size);
};
