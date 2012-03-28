#include "CpuState.h"


CpuState::CpuState(void)
{
}


CpuState::~CpuState(void)
{
}

bool CpuState::RunInstruction()
{
  if (super::RunInstruction())
    return true;
  return false;
}
