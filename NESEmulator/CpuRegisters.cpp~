//#include "StdAfx.h"
#include "CpuRegisters.h"


CpuRegisters::CpuRegisters(void)
{
	A=0;
	X=0;
	Y=0;
	S=0;
	PC=0;
	cycles = 0;

	C = false; // Carry flag
	Z = false; // Zero flag
	I = false; // Interrupt flag
	D = false; // Decimal flag, not used
	B = false; // BRK flag
	V = false; // Overflow flag
	S = false; // Sign flag (1 if negative, 0 if positive)
}


CpuRegisters::~CpuRegisters(void)
{
}

bool CpuRegisters::RunInstruction()
{
        byte opcode = memory->readByteFrom(PC);
	byte arg1 = memory->readByteFrom(PC+1);
	byte arg2 = memory->readByteFrom(PC+2);
	switch (opcode)
	{
	case TAX:
	  PC += 1;
	  cycles += 2;
	  X = A;
	  setNZ(X);
	  break;
	case TAY:
	  PC += 1;
	  cycles += 2;
	  Y = A;
	  setNZ(Y);
	  break;
	case TSX:
	  PC += 1;
	  cycles += 2;
	  X = S;
	  setNZ(X);
	  break;
	case TXA:
	  PC += 1;
	  cycles += 2;
	  A = X;
	  setNZ(A);
	  break;
	case TXS:
	  PC += 1;
	  cycles += 2;
	  S = X;
	  setNZ(S);
	  break;
	case TYA:
	  PC += 1;
	  cycles += 2;
	  A = Y;
	  setNZ(A);
	  break;
	default:
	  return false;
	}
	return true;
}

void CpuRegisters::setNZ(int value)
{
  N = (((int)value) < 0);
  Z = (value == 0);
}
