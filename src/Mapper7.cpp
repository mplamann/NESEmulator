#include "Mapper7.h"
#include "Util.h"2

Mapper7::Mapper7(char* file) : Mapper (file,32,8)
{
}

void Mapper7::writeByteTo(int address, int value)
{
  prgIndexes[0] = value & 0x7;
  mirroring = (value & 0x10) ? MIRRORING_LOWER_BANK : MIRRORING_UPPER_BANK;
}
