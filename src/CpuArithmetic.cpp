#include "CpuArithmetic.h"

CpuArithmetic::CpuArithmetic(void)
{
}

CpuArithmetic::~CpuArithmetic(void)
{
}

bool CpuArithmetic::RunInstruction()
{
  if (super::RunInstruction())
    return true;
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);
  int value = 0;
  switch (opcode)
    {
    case ADC_Imm:
      PC += 2;
      cycles += 2;
      addToA(arg1);
      break;
    case ADC_Zp:
      PC += 2;
      cycles += 3;
      addToA(memory->readByteFrom(addrZp(arg1,arg2)));
      break;
    case ADC_Zpx:
      PC += 2;
      cycles += 4;
      addToA(memory->readByteFrom(addrZpx(arg1,arg2)));
      break;
    case ADC_Abs:
      cycles += 4;
      PC += 3;
      addToA(memory->readByteFrom(addrAbs(arg1,arg2)));
      break;
    case ADC_Absx:
      cycles += 4;
      PC += 3;
      addToA(memory->readByteFrom(addrAbsx(arg1,arg2)));
      break;
    case ADC_Absy:
      cycles += 4;
      PC += 3;
      addToA(memory->readByteFrom(addrAbsy(arg1,arg2)));
      break;
    case ADC_Indx:
      PC += 2;
      cycles += 6;
      addToA(memory->readByteFrom(addrIndx(arg1,arg2)));
      break;
    case ADC_Indy:
      PC += 2;
      cycles += 5;
      addToA(memory->readByteFrom(addrIndy(arg1,arg2)));
      break;

    case SBC_Imm:
    case SBC_Imm2:
      PC += 2;
      cycles += 2;
      subFromA(arg1);
      break;
    case SBC_Zp:
      PC += 2;
      cycles += 3;
      subFromA(memory->readByteFrom(addrZp(arg1,arg2)));
      break;
    case SBC_Zpx:
      PC += 2;
      cycles += 4;
      subFromA(memory->readByteFrom(addrZpx(arg1,arg2)));
      break;
    case SBC_Abs:
      PC += 3;
      cycles += 4;
      subFromA(memory->readByteFrom(addrAbs(arg1,arg2)));
      break;
    case SBC_Absx:
      PC += 3;
      cycles += 4;
      subFromA(memory->readByteFrom(addrAbsx(arg1,arg2)));
      break;
    case SBC_Absy:
      PC += 3;
      cycles += 4;
      subFromA(memory->readByteFrom(addrAbsy(arg1,arg2)));
      break;
    case SBC_Indx:
      PC += 2;
      cycles += 6;
      subFromA(memory->readByteFrom(addrIndx(arg1,arg2)));
      break;
    case SBC_Indy:
      PC += 2;
      cycles += 5;
      subFromA(memory->readByteFrom(addrIndy(arg1,arg2)));
      break;

    case CMP_Imm:
      PC += 2;
      cycles += 2;
      cmpMReg(arg1,A);
      break;
    case CMP_Zp:
      PC += 2;
      cycles += 3;
      cmpMReg(memory->readByteFrom(addrZp(arg1,arg2)),A);
      break;
    case CMP_Zpx:
      PC += 2;
      cycles += 4;
      cmpMReg(memory->readByteFrom(addrZpx(arg1,arg2)),A);
      break;
    case CMP_Abs:
      cycles += 4;
      PC += 3;
      cmpMReg(memory->readByteFrom(addrAbs(arg1,arg2)),A);
      break;
    case CMP_Absx:
      cycles += 4;
      PC += 3;
      cmpMReg(memory->readByteFrom(addrAbsx(arg1,arg2)),A);
      break;
    case CMP_Absy:
      cycles += 4;
      PC += 3;
      cmpMReg(memory->readByteFrom(addrAbsy(arg1,arg2)),A);
      break;
    case CMP_Indx:
      PC += 2;
      cycles += 6;
      cmpMReg(memory->readByteFrom(addrIndx(arg1,arg2)),A);
      break;
    case CMP_Indy:
      PC += 2;
      cycles += 5;
      cmpMReg(memory->readByteFrom(addrIndy(arg1,arg2)),A);
      break;

    case CPX_Imm:
      PC += 2;
      cycles += 2;
      cmpMReg(arg1,X);
      break;
    case CPX_Zp:
      PC += 2;
      cycles += 3;
      cmpMReg(memory->readByteFrom(addrZp(arg1,arg2)),X);
      break;
    case CPX_Abs:
      PC += 3;
      cycles += 4;
      cmpMReg(memory->readByteFrom(addrAbs(arg1,arg2)),X);
      break;

      
    case CPY_Imm:
      PC += 2;
      cycles += 2;
      cmpMReg(arg1,Y);
      break;
    case CPY_Zp:
      PC += 2;
      cycles += 3;
      cmpMReg(memory->readByteFrom(addrZp(arg1,arg2)),Y);
      break;
    case CPY_Abs:
      PC += 3;
      cycles += 4;
      cmpMReg(memory->readByteFrom(addrAbs(arg1,arg2)),Y);
      break;

    case DCP_Zp:
      PC += 2;
      cycles += 5;
      DCP(addrZp(arg1,arg2));
      break;
    case DCP_Zpx:
      PC += 2;
      cycles += 6;
      DCP(addrZpx(arg1,arg2));
      break;
    case DCP_Abs:
      PC += 3;
      cycles += 6;
      DCP(addrAbs(arg1,arg2));
      break;
    case DCP_Absx:
      PC += 3;
      cycles += 7;
      DCP(addrAbsx(arg1,arg2));
      break;
    case DCP_Absy:
      PC += 3;
      cycles += 7;
      DCP(addrAbsy(arg1,arg2));
      break;
    case DCP_Indx:
      PC += 2;
      cycles += 8;
      DCP(addrIndx(arg1,arg2));
      break;
    case DCP_Indy:
      PC += 2;
      cycles += 8;
      DCP(addrIndy(arg1,arg2));
      break;

    case DEC_Zp:
      PC += 2;
      cycles += 5;
      decMemory(addrZp(arg1,arg2));
      break;
    case DEC_Zpx:
      PC += 2;
      cycles += 6;
      decMemory(addrZpx(arg1,arg2));
      break;
    case DEC_Abs:
      PC += 3;
      cycles += 6;
      decMemory(addrAbs(arg1,arg2));
      break;
    case DEC_Absx:
      PC += 3;
      cycles += 7;
      decMemory(addrAbsx(arg1,arg2));
      break;

    case DEX:
      PC += 1;
      cycles += 2;
      X--;
      X &= 0xFF;
      setNZ(X);
      break;
    case DEY:
      PC += 1;
      cycles += 2;
      Y--;
      Y &= 0xFF;
      setNZ(Y);
      break;

    case INC_Zp:
      PC += 2;
      cycles += 5;
      incMemory(addrZp(arg1,arg2));
      break;
    case INC_Zpx:
      PC += 2;
      cycles += 6;
      incMemory(addrZpx(arg1,arg2));
      break;
    case INC_Abs:
      PC += 3;
      cycles += 6;
      incMemory(addrAbs(arg1,arg2));
      break;
    case INC_Absx:
      PC += 3;
      cycles += 7;
      incMemory(addrAbsx(arg1,arg2));
      break;

    case INX:
      PC += 1;
      cycles += 2;
      X++;
      X &= 0xFF;
      setNZ(X);
      break;
    case INY:
      PC += 1;
      cycles += 2;
      Y++;
      Y &= 0xFF;
      setNZ(Y);
      break;

    case ISB_Zp:
      PC += 2;
      cycles += 5;
      ISB(addrZp(arg1,arg2));
      break;
    case ISB_Zpx:
      PC += 2;
      cycles += 6;
      ISB(addrZpx(arg1,arg2));
      break;
    case ISB_Abs:
      PC += 3;
      cycles += 6;
      ISB(addrAbs(arg1,arg2));
      break;
    case ISB_Absx:
      PC += 3;
      cycles += 7;
      ISB(addrAbsx(arg1,arg2));
      break;
    case ISB_Absy:
      PC += 3;
      cycles += 7;
      ISB(addrAbsy(arg1,arg2));
      break;
    case ISB_Indx:
      PC += 2;
      cycles += 8;
      ISB(addrIndx(arg1,arg2));
      break;
    case ISB_Indy:
      PC += 2;
      cycles += 8;
      ISB(addrIndy(arg1,arg2));
      break;

    case RRA_Zp:
      PC += 2;
      cycles += 5;
      RRA(addrZp(arg1,arg2));
      break;
    case RRA_Zpx:
      PC += 2;
      cycles += 6;
      RRA(addrZpx(arg1,arg2));
      break;
    case RRA_Abs:
      PC += 3;
      cycles += 6;
      RRA(addrAbs(arg1,arg2));
      break;
    case RRA_Absx:
      PC += 3;
      cycles += 7;
      RRA(addrAbsx(arg1,arg2));
      break;
    case RRA_Absy:
      PC += 3;
      cycles += 7;
      RRA(addrAbsy(arg1,arg2));
      break;
    case RRA_Indx:
      PC += 2;
      cycles += 8;
      RRA(addrIndx(arg1,arg2));
      break;
    case RRA_Indy:
      PC += 2;
      cycles += 8;
      RRA(addrIndy(arg1,arg2));
      break;
      
    default:
      return false;
    }
  return true;
}

void CpuArithmetic::addToA(int value)
{
  unsigned int temp = value + A + (C ? 1 : 0);
  C = (temp > 0xFF);
  V = (!((A ^ value) & 0x80) && ((A ^ temp) & 0x80));
  A = (temp & 0xFF);
  N = ((A & 0x80) != 0);
  Z = (A == 0);
}

void CpuArithmetic::subFromA(int value)
{
  unsigned int temp = A - value - (C ? 0 : 1);
  C = (temp < 0x100);
  V = (((A ^ value) & 0x80) && ((A ^ temp) & 0x80));
  A = temp & 0xFF;
  N = ((A & 0x80) != 0);
  Z = (A == 0);
}

void CpuArithmetic::cmpMReg(int value, int reg)
{
  unsigned int temp = reg - value;
  C = (temp < 0x100);
  N = ((temp & 0x80) != 0);
  Z = (temp == 0);
}

void CpuArithmetic::decMemory(int address)
{
  int value = memory->readByteFrom(address);
  value--;
  memory->writeByteTo(address, value);
  setNZ(value);
}

void CpuArithmetic::incMemory(int address)
{
  int value = memory->readByteFrom(address);
  value++;
  memory->writeByteTo(address,value);
  setNZ(value);
}

void CpuArithmetic::DCP(int address)
{
  decMemory(address);
  cmpMReg(memory->readByteFrom(address),A);
}

void CpuArithmetic::ISB(int address)
{
  incMemory(address);
  subFromA(memory->readByteFrom(address));
}

void CpuArithmetic::RRA(int address)
{
  int value = ROR(memory->readByteFrom(address));
  memory->writeByteTo(address, value);
  addToA(value);
}
