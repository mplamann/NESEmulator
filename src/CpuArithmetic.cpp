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

    case DEC_Zp:
      PC += 2;
      cycles += 5;
      value = memory->readByteFrom(addrZp(arg1,arg2));
      value--;
      memory->writeByteTo(addrZp(arg1,arg2), value);
      setNZ(value);
      break;
    case DEC_Zpx:
      PC += 2;
      cycles += 6;
      value = memory->readByteFrom(addrZpx(arg1,arg2));
      value--;
      memory->writeByteTo(addrZpx(arg1,arg2), value);
      setNZ(value);
      break;
    case DEC_Abs:
      PC += 3;
      cycles += 6;
      value = memory->readByteFrom(addrAbs(arg1,arg2));
      value--;
      memory->writeByteTo(addrAbs(arg1,arg2), value);
      setNZ(value);
      break;
    case DEC_Absx:
      PC += 3;
      cycles += 7;
      value = memory->readByteFrom(addrAbsx(arg1,arg2));
      value--;
      memory->writeByteTo(addrAbsx(arg1,arg2), value);
      setNZ(value);
      break;

    case DEX:
      PC += 1;
      cycles += 2;
      X--;
      setNZ(X);
      break;
    case DEY:
      PC += 1;
      cycles += 2;
      Y--;
      setNZ(Y);
      break;

    case INC_Zp:
      PC += 2;
      cycles += 5;
      value = memory->readByteFrom(addrZp(arg1,arg2));
      value++;
      memory->writeByteTo(addrZp(arg1,arg2),value);
      setNZ(value);
      break;
    case INC_Zpx:
      PC += 2;
      cycles += 6;
      value = memory->readByteFrom(addrZpx(arg1,arg2));
      value++;
      memory->writeByteTo(addrZp(arg1,arg2),value);
      setNZ(value);
      break;
    case INC_Abs:
      PC += 3;
      cycles += 6;
      value = memory->readByteFrom(addrAbs(arg1,arg2));
      value++;
      memory->writeByteTo(addrAbs(arg1,arg2));
      setNZ(value);
      break;
    case INC_Absx:
      PC += 3;
      cycles += 7;
      value = memory->readByteFrom(addrAbs(arg1,arg2));
      value++;
      memory->writeByteTo(addrAbsx(arg1,arg2));
      setNZ(value);
      break;

    case INX:
      PC += 1;
      cycles += 2;
      X++;
      setNZ(X);
      break;
    case INY:
      PC += 1;
      cycles += 2;
      Y++;
      setNZ(Y);
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
  N = (((int)temp) < 0);
  Z = (temp == 0);
  V = (!((A ^ value) & 0x80) && ((A ^ temp) & 0x80));
  A = temp;
}

void CpuArithmetic::subFromA(int value)
{
  unsigned int temp = A - value - (C ? 0 : 1);
  C = (temp < 0x100);
  N = (((int)temp) < 0);
  Z = (temp == 0);
  V = (((A ^ value) & 0x80) && ((A ^ temp) & 0x80));
  A = temp & 0xFF;
}
