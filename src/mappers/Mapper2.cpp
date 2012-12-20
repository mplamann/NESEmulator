#include "Mapper2.h"
#include <iostream>
using namespace std;

Mapper2::Mapper2(char* file) : Mapper(file)
{
  cout << "using mapper 2...";
  prgBank1Index = 0;
  prgBank2Index = nPrgBanks-1;
}

Mapper2::~Mapper2(void)
{
}

void Mapper2::writeByteTo(int address, int value)
{
  /*if (address < 0x8000)
    {
      cout << "Mapper2 cannot write to addresses < 0x8000.\n";
      return;
      }*/
  prgBank1Index = value;
}
