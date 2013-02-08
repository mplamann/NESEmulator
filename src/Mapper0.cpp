#include "Mapper0.h"
#include <iostream>
using namespace std;

Mapper0::Mapper0(char* file) : Mapper(file)
{
  cout << "using mapper 0...";
}

Mapper0::~Mapper0(void)
{
}

void Mapper0::countScanline() {}
