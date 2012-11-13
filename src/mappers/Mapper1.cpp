#include "Mapper1.h"
#include <iostream>
using namespace std;

Mapper1::Mapper1(char* file) : Mapper(file)
{
  cout << "using mapper 1...";
}

Mapper1::~Mapper1(void)
{
}

void Mapper1::writeByteTo(int address, int value)
{
  
}
