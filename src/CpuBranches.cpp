#include "CpuBranches.h"

#include <iostream>
using namespace std;

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
    case BMI:
      doBranch(N,arg1);
      break;
    case BNE:
      doBranch(!Z,arg1);
      break;
    case BPL:
      doBranch(!N,arg1);
      break;
    case BVC:
      doBranch(!V,arg1);
      break;
    case BVS:
      doBranch(V,arg1);
      break;
    case JMP_Abs:
      doJump(addrAbs(arg1,arg2));
      break;
    case JMP_Ind:
      doJump(addrInd(arg1,arg2));
      cycles += 2;
      break;
    case JSR:
      pushToStack(((PC+2) >> 8) & 0xFF);
      pushToStack((PC+2) & 0xFF);
      cycles += 3;
      doJump(addrAbs(arg1,arg2));
      break;
    case RTS:
      PC = popFromStack() + (popFromStack() << 8);
      PC++;
      cycles += 6;
      break;
    case RTI:
      setP(popFromStack());
      PC = popFromStack() + (popFromStack() << 8);
      cycles += 6;
      break;
    case BRK:
      PC += 2;
      cycles += 7;
      doBRK();
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

void CpuBranches::doJump(int target)
{
  cycles += 3;
  PC = target;
  // Yay! that was a fun function!
  // Necessary too!
}

// for NMI or IRQ/BRK
void CpuBranches::processInterrupt(int vector)
{
  pushToStack((PC >> 8) & 0xFF);
  pushToStack(PC & 0xFF);
  pushToStack(getP());
  I = true;
  PC = vector;
}

void CpuBranches::doRESET()
{
  cycles += 6;
  I = true;
  PC = memory->readByteFrom(VECTOR_RESET) + (memory->readByteFrom(VECTOR_RESET+1) << 8);
}

void CpuBranches::doNMI()
{
  //  cout << "NMI!\n";
  int vector = memory->readByteFrom(VECTOR_NMI) + (memory->readByteFrom(VECTOR_NMI+1) << 8);
  processInterrupt(vector);
}

void CpuBranches::doBRK()
{
  int vector = memory->readByteFrom(VECTOR_BRK) + (memory->readByteFrom(VECTOR_BRK+1) << 8);
  processInterrupt(vector);
}
