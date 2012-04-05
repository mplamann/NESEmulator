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
    case BCC:
      doBranch(!C,arg1);
      break;
    case BCS:
      doBranch(C,arg1);
      break;
    case BEQ:
      doBranch(Z,arg1);
      break;
    default:
      return false;
    }
  return true;
}

void CpuBranches::doBranch(bool condition, int offset)
{
  cycles += 2;
  PC += 2;
  if (!condition)
    return;
  if ((offset & 0x7F) != offset)
    {
      offset = (0x7F & offset) - 0x80; // Because we are dealing with 8-bit signed ints
    }
  if (pageBoundaryCrossed(PC, offset))
    cycles += 2;
  else
    cycles += 1;
  PC += offset;
}

