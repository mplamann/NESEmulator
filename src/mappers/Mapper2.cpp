#include "Mapper2.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
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

void Mapper2::writeByteTo(int, int value)
{
  /*if (address < 0x8000)
    {
      cout << "Mapper2 cannot write to addresses < 0x8000.\n";
      return;
      }*/
  prgBank1Index = value;
}

size_t Mapper2::stateSize()
{
  int sInts = sizeof(int)*4;
  return sInts;
}

char* Mapper2::stateData()
{
  char* buffer = (char*)malloc(sizeof(char)*stateSize());
  int otherValues[4] = {prgBank1Index, prgBank2Index, chrBank1Index, chrBank2Index};
  memcpy(buffer, otherValues, 4*sizeof(int));
  return buffer;
}

void Mapper2::loadState(char* buffer)
{
  int otherValues[4];
  memcpy(otherValues, buffer, 4*sizeof(int));
  prgBank1Index = otherValues[0];
  prgBank2Index = otherValues[1];
  chrBank1Index = otherValues[2];
  chrBank2Index = otherValues[3];
}

