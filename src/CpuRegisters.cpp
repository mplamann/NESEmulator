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
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);
  int zpAddress = 0;
  int indirectAddress = 0;
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

    case LDA_Imm:
      PC += 2;
      A = arg1;
      cycles += 2;
      setNZ(A);
      break;
    case LDA_Zp:
      PC += 2;
      A = memory->readByteFrom(arg1);
      cycles += 3;
      setNZ(A);
      break;
    case LDA_Zpx:
      PC += 2;
      cycles += 4;
      A = memory->readByteFrom(arg1+X);
      setNZ(A);
      break;
    case LDA_Abs:
      LDAAbs(arg1, arg2, 0);
      break;
    case LDA_Absx:
      LDAAbs(arg1, arg2, X);
      break;
    case LDA_Absy:
      LDAAbs(arg1, arg2, Y);
      break;
    case LDA_Indx:
      PC += 2;
      cycles += 6;
      zpAddress = (arg1 + X) % 256;
      indirectAddress = (memory->readByteFrom(zpAddress) + (memory->readByteFrom(zpAddress+1)<<8));
      A = memory->readByteFrom(indirectAddress);
      setNZ(A);
      break;
    case LDA_Indy:
      PC += 2;
      cycles += 5;
      indirectAddress = memory->readByteFrom(arg1) + (memory->readByteFrom(arg1+1)<<8);
      if (pageBoundaryCrossed(indirectAddress, Y))
	cycles += 1;
      A = memory->readByteFrom(indirectAddress+Y);
      setNZ(A);
      break;

    case LDX_Imm:
      PC += 2;
      cycles += 2;
      X = arg1;
      setNZ(X);
      break;
    case LDX_Zp:
      PC += 2;
      cycles += 3;
      X = memory->readByteFrom(arg1);
      setNZ(X);
      break;
    case LDX_Zpy:
      PC += 2;
      cycles += 4;
      X = memory->readByteFrom(arg1 + Y);
      setNZ(X);
      break;
    case LDX_Abs:
      PC += 3;
      cycles += 4;
      X = memory->readByteFrom(arg1 + (arg2 << 8));
      setNZ(X);
      break;
    case LDX_Absy:
      PC += 3;
      cycles += 4;
      if (pageBoundaryCrossed(arg1+(arg2<<8),Y))
	cycles += 1;
      X = memory->readByteFrom(arg1+(arg2<<8)+Y);
      setNZ(X);
      break;

    case LDY_Imm:
      PC += 2;
      cycles += 2;
      Y = arg1;
      setNZ(Y);
      break;
    case LDY_Zp:
      PC += 2;
      cycles += 3;
      Y = memory->readByteFrom(arg1);
      setNZ(Y);
      break;
    case LDY_Zpx:
      PC += 2;
      cycles += 4;
      Y = memory->readByteFrom(arg1 + X);
      setNZ(Y);
      break;
    case LDY_Abs:
      PC += 3;
      cycles += 4;
      Y = memory->readByteFrom(arg1 + (arg2 << 8));
      setNZ(Y);
      break;
    case LDY_Absx:
      PC += 3;
      cycles += 4;
      if (pageBoundaryCrossed(arg1+(arg2<<8),X))
	cycles += 1;
      Y = memory->readByteFrom(arg1+(arg2<<8)+X);
      setNZ(Y);
      break;
    default:
      return false;
    }
  return true;
}

void CpuRegisters::LDAAbs(int arg1, int arg2, int offset)
{
  PC += 3;
  cycles += 4;
  if (pageBoundaryCrossed(arg1+(arg2<<8),offset))
    cycles += 1;
  A = memory->readByteFrom(arg1+(arg2<<8)+offset);
  setNZ(A);
}

void CpuRegisters::setNZ(int value)
{
  N = (((int)value) < 0);
  Z = (value == 0);
}

bool CpuRegisters::pageBoundaryCrossed(int address, int offset)
{
	int newAddress = address + offset;
	if ((newAddress & 0xF0) != (address & 0xF0))
		return true;
	return false;
}
