#include "CpuState.h"


CpuState::CpuState(void)
{
  memory = new MemoryState();
}


CpuState::~CpuState(void)
{
  delete memory;
}

bool CpuState::RunInstruction()
{
  if (super::RunInstruction())
    return true;
  return false;
}
