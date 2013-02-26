#pragma once

#include "MemoryState.h"
#include <fstream>
using namespace std;

class CpuV2
{
private:
  float accumulator;
public:
  int A;
  int X;
  int Y;
  int S;
  int PC;

  bool N,Z,C,I,D,V,B; // Processor status flags
  int CLIdelay;
  int getP();
  void setP(int P);

  long cycles;
  int total_cycles;
  int cycles_remain;
  int elapsed();
  MemoryState* memory;
  
  CpuV2(void);
  ~CpuV2(void);
  void RunInstruction(int scanline);
  void RunForCycles(float cycle_cout, int scanline);

  void doRESET();
  void doNMI();
  void doBRK();
  void doIRQ();

  void saveState(ofstream& file);
  void loadState(ifstream& file);
};
