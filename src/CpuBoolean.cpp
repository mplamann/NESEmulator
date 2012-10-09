#include "CpuBoolean.h"

CpuBoolean::CpuBoolean(void)
{
}

CpuBoolean::~CpuBoolean(void)
{
}

bool CpuBoolean::RunInstruction()
{
  if (super::RunInstruction())
    return true;
  int opcode = memory->readByteFrom(PC);
  int arg1 = memory->readByteFrom(PC+1);
  int arg2 = memory->readByteFrom(PC+2);
  int address = 0;
  switch (opcode)
    {
    case AND_Imm:
      PC += 2;
      cycles += 2;
      A &= arg1;
      setNZ(A);
      break;
    case AND_Zp:
      PC += 2;
      cycles += 3;
      A &= memory->readByteFrom(addrZp(arg1,arg2));
      setNZ(A);
      break;
    case AND_Zpx:
      PC += 2;
      cycles += 4;
      A &= memory->readByteFrom(addrZpx(arg1,arg2));
      setNZ(A);
      break;
    case AND_Abs:
      PC += 3;
      cycles += 4;
      A &= memory->readByteFrom(addrAbs(arg1,arg2));
      setNZ(A);
      break;
    case AND_Absx:
      PC += 3;
      cycles += 4;
      A &= memory->readByteFrom(addrAbsx(arg1,arg2));
      setNZ(A);
      break;
    case AND_Absy:
      PC += 3;
      cycles += 4;
      A &= memory->readByteFrom(addrAbsy(arg1,arg2));
      setNZ(A);
      break;
    case AND_Indx:
      PC += 2;
      cycles += 6;
      A &= memory->readByteFrom(addrIndx(arg1,arg2));
      setNZ(A);
      break;
    case AND_Indy:
      PC += 2;
      cycles += 5;
      A &= memory->readByteFrom(addrIndy(arg1,arg2));
      setNZ(A);
      break;

    case EOR_Imm:
      PC += 2;
      cycles += 2;
      A ^= arg1;
      setNZ(A);
      break;
    case EOR_Zp:
      PC += 2;
      cycles += 3;
      A ^= memory->readByteFrom(addrZp(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Zpx:
      PC += 2;
      cycles += 4;
      A ^= memory->readByteFrom(addrZpx(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Abs:
      PC += 3;
      cycles += 4;
      A ^= memory->readByteFrom(addrAbs(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Absx:
      PC += 3;
      cycles += 4;
      A ^= memory->readByteFrom(addrAbsx(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Absy:
      PC += 3;
      cycles += 4;
      A ^= memory->readByteFrom(addrAbsy(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Indx:
      PC += 2;
      cycles += 6;
      A ^= memory->readByteFrom(addrIndx(arg1,arg2));
      setNZ(A);
      break;
    case EOR_Indy:
      PC += 2;
      cycles += 5;
      A ^= memory->readByteFrom(addrIndy(arg1,arg2));
      setNZ(A);
      break;

    case ORA_Imm:
      PC += 2;
      cycles += 2;
      A |= arg1;
      setNZ(A);
      break;
    case ORA_Zp:
      PC += 2;
      cycles += 3;
      A |= memory->readByteFrom(addrZp(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Zpx:
      PC += 2;
      cycles += 4;
      A |= memory->readByteFrom(addrZpx(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Abs:
      PC += 3;
      cycles += 4;
      A |= memory->readByteFrom(addrAbs(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Absx:
      PC += 3;
      cycles += 4;
      A |= memory->readByteFrom(addrAbsx(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Absy:
      PC += 3;
      cycles += 4;
      A |= memory->readByteFrom(addrAbsy(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Indx:
      PC += 2;
      cycles += 6;
      A |= memory->readByteFrom(addrIndx(arg1,arg2));
      setNZ(A);
      break;
    case ORA_Indy:
      PC += 2;
      cycles += 5;
      A |= memory->readByteFrom(addrIndy(arg1,arg2));
      setNZ(A);
      break;

    case ASL_A:
      PC += 1;
      cycles += 2;
      A = ASL(A);
      break;
    case ASL_Zp:
      PC += 2;
      cycles += 5;
      memory->writeByteTo(addrZp(arg1,arg2),ASL(memory->readByteFrom(addrZp(arg1,arg2))));
      break;
    case ASL_Zpx:
      PC += 2;
      cycles += 6;
      memory->writeByteTo(addrZpx(arg1,arg2),ASL(memory->readByteFrom(addrZpx(arg1,arg2))));
      break;
    case ASL_Abs:
      PC += 3;
      cycles += 6;
      memory->writeByteTo(addrAbs(arg1,arg2),ASL(memory->readByteFrom(addrAbs(arg1,arg2))));
      break;
    case ASL_Absx:
      PC += 3;
      cycles += 7;
      memory->writeByteTo(addrAbsx(arg1,arg2),ASL(memory->readByteFrom(addrAbsx(arg1,arg2))));
      break;

    case LSR_A:
      PC += 1;
      cycles += 2;
      A = LSR(A);
      break;
    case LSR_Zp:
      PC += 2;
      cycles += 5;
      address = addrZp(arg1,arg2);
      memory->writeByteTo(address,LSR(memory->readByteFrom(address)));
      break;
    case LSR_Zpx:
      PC += 2;
      cycles += 6;
      address = addrZpx(arg1,arg2);
      memory->writeByteTo(address,LSR(memory->readByteFrom(address)));
      break;
    case LSR_Abs:
      PC += 3;
      cycles += 6;
      address = addrAbs(arg1,arg2);
      memory->writeByteTo(address,LSR(memory->readByteFrom(address)));
      break;
    case LSR_Absx:
      PC += 3;
      cycles += 7;
      address = addrAbsx(arg1,arg2);
      memory->writeByteTo(address,LSR(memory->readByteFrom(address)));
      break;

    case ROL_A:
      PC += 1;
      cycles += 2;
      A = ROL(A);
      break;
    case ROL_Zp:
      PC += 2;
      cycles += 5;
      address = addrZp(arg1,arg2);
      memory->writeByteTo(address,ROL(memory->readByteFrom(address)));
      break;
    case ROL_Zpx:
      PC += 2;
      cycles += 6;
      address = addrZpx(arg1,arg2);
      memory->writeByteTo(address,ROL(memory->readByteFrom(address)));
      break;
    case ROL_Abs:
      PC += 3;
      cycles += 6;
      address = addrAbs(arg1,arg2);
      memory->writeByteTo(address,ROL(memory->readByteFrom(address)));
      break;
    case ROL_Absx:
      PC += 3;
      cycles += 7;
      address = addrAbsx(arg1,arg2);
      memory->writeByteTo(address,ROL(memory->readByteFrom(address)));
      break;

    case ROR_A:
      PC += 1;
      cycles += 2;
      A = ROR(A);
      break;
    case ROR_Zp:
      PC += 2;
      cycles += 5;
      address = addrZp(arg1,arg2);
      memory->writeByteTo(address,ROR(memory->readByteFrom(address)));
      break;
    case ROR_Zpx:
      PC += 2;
      cycles += 6;
      address = addrZpx(arg1,arg2);
      memory->writeByteTo(address,ROR(memory->readByteFrom(address)));
      break;
    case ROR_Abs:
      PC += 3;
      cycles += 6;
      address = addrAbs(arg1,arg2);
      memory->writeByteTo(address,ROR(memory->readByteFrom(address)));
      break;
    case ROR_Absx:
      PC += 3;
      cycles += 7;
      address = addrAbsx(arg1,arg2);
      memory->writeByteTo(address,ROR(memory->readByteFrom(address)));
      break;
      
    case BIT_Zp:
      PC += 2;
      cycles += 3;
      BIT(memory->readByteFrom(addrZp(arg1,arg2)));
      break;
    case BIT_Abs:
      PC += 3;
      cycles += 4;
      BIT(memory->readByteFrom(addrAbs(arg1,arg2)));
      break;
    default:
      return false;
    }
  return true;
}

int CpuBoolean::ASL(int arg)
{
  int value = arg << 1;
  /*if ((value & 0xFF) != (value))
    C = true;
  else
  C = false;*/
  C = (arg&0x80);
  value = (value & 0xFF);
  setNZ(value);
  return value;
}

int CpuBoolean::LSR(int arg)
{
  int value = arg>>1;
  /*if (arg % 2 == 1)
    C = true;
  else
  C = false;*/
  C = arg&0x01;
  value &= 0x7F; // Bit 7 will always be 0 anyways
  setNZ(value);
  return value;
}

int CpuBoolean::ROL(int arg)
{
  int value = (arg << 1);
  value += (int)C;
  C = (value > 0xFF);
  value &= 0xFF;
  setNZ(value);
  return value;
}

int CpuBoolean::ROR(int arg)
{
  if (C)
    arg |= 0x100;
  C = (arg & 0x1);
  int value = (arg >> 1);
  value &= 0xFF;
  setNZ(value);
  return value;
}

void CpuBoolean::BIT(int arg)
{
  Z = ((A & arg) == 0);
  V = ((0x40 & arg) != 0);
  N = ((0x80 & arg) != 0);
}
