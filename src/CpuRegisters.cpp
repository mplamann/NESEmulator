#include "CpuRegisters.h"
#include <iostream>
using namespace std;

CpuRegisters::CpuRegisters(void)
{
  A=0;
  X=0;
  Y=0;
  S=0xFF;
  PC=0;
  cycles = 0;

  C = false; // Carry flag
  Z = false; // Zero flag
  I = true; // Interrupt flag
  D = false; // Decimal flag, not used
  B = false; // BRK flag
  V = false; // Overflow flag
  N = false; // Sign flag (1 if negative, 0 if positive)
}


CpuRegisters::~CpuRegisters(void)
{
}

bool CpuRegisters::RunInstruction()
{
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);
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
      A = memory->readByteFrom(addrZp(arg1,arg2));
      cycles += 3;
      setNZ(A);
      break;
    case LDA_Zpx:
      PC += 2;
      cycles += 4;
      A = memory->readByteFrom(addrZpx(arg1,arg2));
      setNZ(A);
      break;
    case LDA_Abs:
      PC += 3;
      cycles += 4;
      A = memory->readByteFrom(addrAbs(arg1,arg2));
      break;
    case LDA_Absx:
      PC += 3;
      cycles += 4;
      A = memory->readByteFrom(addrAbsx(arg1,arg2));
      setNZ(A);
      break;
    case LDA_Absy:
      PC += 3;
      cycles += 4;
      A = memory->readByteFrom(addrAbsy(arg1,arg2));
      setNZ(A);
      break;
    case LDA_Indx:
      PC += 2;
      cycles += 6;
      A = memory->readByteFrom(addrIndx(arg1,arg2));
      setNZ(A);
      break;
    case LDA_Indy:
      PC += 2;
      cycles += 5;
      A = memory->readByteFrom(addrIndy(arg1,arg2));
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
      X = memory->readByteFrom(addrZp(arg1,arg2));
      setNZ(X);
      break;
    case LDX_Zpy:
      PC += 2;
      cycles += 4;
      X = memory->readByteFrom(addrZpy(arg1,arg2));
      setNZ(X);
      break;
    case LDX_Abs:
      PC += 3;
      cycles += 4;
      X = memory->readByteFrom(addrAbs(arg1,arg2));
      setNZ(X);
      break;
    case LDX_Absy:
      PC += 3;
      cycles += 4;
      X = memory->readByteFrom(addrAbsy(arg1,arg2));
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
      Y = memory->readByteFrom(addrZp(arg1,arg2));
      setNZ(Y);
      break;
    case LDY_Zpx:
      PC += 2;
      cycles += 4;
      Y = memory->readByteFrom(addrZpx(arg1,arg2));
      setNZ(Y);
      break;
    case LDY_Abs:
      PC += 3;
      cycles += 4;
      Y = memory->readByteFrom(addrAbs(arg1,arg2));
      setNZ(Y);
      break;
    case LDY_Absx:
      PC += 3;
      cycles += 4;
      Y = memory->readByteFrom(addrAbsx(arg1,arg2));
      setNZ(Y);
      break;

    case STA_Zp:
      PC += 2;
      cycles += 3;
      memory->writeByteTo(addrZp(arg1,arg2),A);
      break;
    case STA_Zpx:
      PC += 2;
      cycles += 4;
      memory->writeByteTo(addrZpx(arg1,arg2),A);
      break;
    case STA_Abs:
      PC += 3;
      cycles += 4;
      memory->writeByteTo(addrAbs(arg1,arg2),A);
      break;
    case STA_Absx:
      PC += 3;
      cycles += 5;
      memory->writeByteTo(addrAbsx(arg1,arg2),A);
      break;
    case STA_Absy:
      PC += 3;
      cycles += 5;
      memory->writeByteTo(addrAbsy(arg1,arg2),A);
      break;
    case STA_Indx:
      PC += 2;
      cycles += 6;
      memory->writeByteTo(addrIndx(arg1,arg2),A);
      break;
    case STA_Indy:
      PC += 2;
      cycles += 6;
      memory->writeByteTo(addrIndy(arg1,arg2),A);
      break;

    case STX_Zp:
      PC += 2;
      cycles += 3;
      memory->writeByteTo(addrZp(arg1,arg2),X);
      break;
    case STX_Zpy:
      PC += 2;
      cycles += 4;
      memory->writeByteTo(addrZpy(arg1,arg2),X);
      break;
    case STX_Abs:
      PC += 3;
      cycles += 4;
      memory->writeByteTo(addrAbs(arg1,arg2),X);
      break;

    case STY_Zp:
      PC += 2;
      cycles += 3;
      memory->writeByteTo(addrZp(arg1,arg2),Y);
      break;
    case STY_Zpx:
      PC += 2;
      cycles += 4;
      memory->writeByteTo(addrZpx(arg1,arg2),Y);
      break;
    case STY_Abs:
      PC += 3;
      cycles += 4;
      memory->writeByteTo(addrAbs(arg1,arg2),Y);
      break;

    case CLC:
      PC += 1;
      cycles += 2;
      C = false;
      break;
    case CLD:
      PC += 1;
      cycles += 2;
      D = false;
      break;
    case CLI:
      PC += 1;
      cycles += 2;
      I = false;
      break;
    case CLV:
      PC += 1;
      cycles += 2;
      V = false;
      break;

    case SEC:
      PC += 1;
      cycles += 2;
      C = true;
      break;
    case SED:
      PC += 1;
      cycles += 2;
      D = true;
      break;
    case SEI:
      PC += 1;
      cycles += 2;
      I = true;
      break;

    case NOP:
      PC += 1;
      cycles += 2;
      break;

    case PHA:
      pushToStack(A);
      PC += 1;
      cycles += 3;
      break;
    case PHP:
      cycles += 3;
      PC += 1;
      pushToStack(getP());
      break;

    case PLA:
      cycles += 4;
      PC += 1;
      A = popFromStack();
      setNZ(A);
      break;
    case PLP:
      cycles += 4;
      PC += 1;
      setP(popFromStack());
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
  value &= 0xFF; // Make sure it is one byte that we are looking at
  N = ((value & 0x80) != 0);
  Z = (value == 0);
}

bool CpuRegisters::pageBoundaryCrossed(int address, int offset)
{
  int newAddress = address + offset;
  if ((newAddress & 0xF0) != (address & 0xF0))
    return true;
  return false;
}

int CpuRegisters::addrZp(int arg1, int arg2)
{
  return arg1;
}

int CpuRegisters::addrZpx(int arg1, int arg2)
{
  return arg1 + X;
}

int CpuRegisters::addrZpy(int arg1, int arg2)
{
  return arg1 + Y;
}

int CpuRegisters::addrAbs(int arg1, int arg2)
{
  return arg1 + (arg2 << 8);
}

int CpuRegisters::addrAbsx(int arg1, int arg2)
{
  if (pageBoundaryCrossed(arg1 + (arg2<<8), X))
    cycles += 1;
  return arg1 + (arg2<<8) + X;
}

int CpuRegisters::addrAbsy(int arg1, int arg2)
{
  if (pageBoundaryCrossed(arg1 + (arg2<<8), X))
    cycles += 1;
  return arg1 + (arg2<<8) + Y;
}

int CpuRegisters::addrIndx(int arg1, int arg2)
{
  int zpAddress = (arg1 + X) % 256;
  int indirectAddress = memory->readByteFrom((zpAddress)) + (memory->readByteFrom(zpAddress+1)<<8);
  return indirectAddress;
}

int CpuRegisters::addrIndy(int arg1, int arg2)
{
  int indirectAddress = memory->readByteFrom(arg1) + (memory->readByteFrom(arg1 + 1) << 8);
  if (pageBoundaryCrossed(indirectAddress,Y))
    cycles += 1;
  return indirectAddress+Y;
}

int CpuRegisters::addrInd(int arg1, int arg2)
{
  int absAddr = arg1 + (arg2 << 8);
  return memory->readByteFrom(absAddr) + (memory->readByteFrom(absAddr + 1) << 8);
}

void CpuRegisters::pushToStack(int value)
{
  memory->writeByteTo(0x100|(S--&0xFF),value);
}

int CpuRegisters::popFromStack()
{
  return memory->readByteFrom(0x100|(++S&0xFF));
}

int CpuRegisters::getA() {return A;}
int CpuRegisters::getX() {return X;}
int CpuRegisters::getY() {return Y;}
int CpuRegisters::getS() {return S;}
int CpuRegisters::getPC() {return PC;}
int CpuRegisters::getP()
{
  int P = 0x20;
  if (C)
    P |= 0x01;
  if (Z)
    P |= 0x02;
  if (I)
    P |= 0x04;
  if (B)
    P |= 0x10;
  if (V)
    P |= 0x40;
  if (N)
    P |= 0x80;
  if (D)
    P |= 0x08;
  return P;
}
int CpuRegisters::getCycles() {return cycles;}
bool CpuRegisters::getN() {return N;}
bool CpuRegisters::getZ() {return Z;}
bool CpuRegisters::getC() {return C;}
bool CpuRegisters::getD() {return D;}
bool CpuRegisters::getI() {return I;}
bool CpuRegisters::getV() {return V;}
bool CpuRegisters::getB() {return B;}
MemoryState* CpuRegisters::getMemory() {return memory;}
void CpuRegisters::setMemory(MemoryState* mem) {memory = mem;}

void CpuRegisters::setX(int value) {X = value;}
void CpuRegisters::setA(int value) {A = value;}
void CpuRegisters::setY(int value) {Y = value;}
void CpuRegisters::setS(int value) {S = value;}
void CpuRegisters::setPC(int value) {PC = value;}
void CpuRegisters::setP(int P)
{
  C = ((P & 0x01) != 0);
  Z = ((P & 0x02) != 0);
  I = ((P & 0x04) != 0);
  B = ((P & 0x10) != 0);
  V = ((P & 0x40) != 0);
  N = ((P & 0x80) != 0);
}
