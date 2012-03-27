#include "CpuBranches.h"

CpuBranches::CpuBranches(void)
{
}

CpuBranches::~CpuBranches(void)
{
}

bool CpuBranches::RunInstruction()
{
  if (super::RunInstruction())
    return true;
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);
  switch (opcode)
    {
    default:
      return false;
    }
  return true;
}
