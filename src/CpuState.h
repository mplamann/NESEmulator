#pragma once

#include "Util.h"
#include "MemoryState.h"
#include "CpuBranches.h"

class CpuState : public CpuBranches
{
private:
  typedef CpuBranches super;
  void printLog();
  float accumulator;
public:
  CpuState(void);
  ~CpuState(void);
  bool RunInstruction();
  void RunForCycles(float cycle_count, int scanline);
  
  char* stateData(size_t* size);
};

