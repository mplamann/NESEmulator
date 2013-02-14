#include "Mapper2.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

Mapper2::Mapper2(char* file) : Mapper(file)
{
  cout << "using mapper 2...";
  prgIndexes[0] = 0;
  prgIndexes[1] = nPrgBanks-1;
}

Mapper2::~Mapper2(void)
{
}

void Mapper2::writeByteTo(int, int value)
{
  prgIndexes[0] = value;
}

size_t Mapper2::stateSize()
{
  return sizeof(int)*(16/prgBankSize + 8/chrBankSize);
}

char* Mapper2::stateData()
{

  char* buffer = (char*)malloc(sizeof(char)*stateSize());

  memcpy(buffer, prgIndexes, 16/prgBankSize*sizeof(int));
  int bufferIndex = sizeof(int)*16/prgBankSize;
  memcpy(buffer+bufferIndex, chrIndexes, 8/chrBankSize*sizeof(int));

  return buffer;
}

void Mapper2::loadState(char* buffer)
{
  memcpy(prgIndexes, buffer, 16/prgBankSize*sizeof(int));
  int bufferIndex = 16/prgBankSize*sizeof(int);
  memcpy(chrIndexes, buffer+bufferIndex, 8/chrBankSize*sizeof(int));
}

