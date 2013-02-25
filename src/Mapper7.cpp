#include "Mapper7.h"
#include "Util.h"

Mapper7::Mapper7(char* file) : Mapper (file,32)
{
  prgIndexes[0] = 0;
}

void Mapper7::writeByteTo(int, int value)
{
  prgIndexes[0] = value & 0x7;
  mirroring = (value & 0x10) ? MIRRORING_UPPER_BANK : MIRRORING_LOWER_BANK;
}
