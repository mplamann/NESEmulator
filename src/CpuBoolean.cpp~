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
    default:
      return false;
    }
  return true;
}
