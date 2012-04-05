#pragma once
#include "Util.h"
#include "MemoryState.h"
class CpuRegisters
{
 private:
  void LDAAbs(int arg1, int arg2, int offset);
 protected:
  int A;
  int X;
  int Y;
  int S;
  int PC;

  bool N,Z,C,I,D,V,B; // Processor status flags

  int cycles;
  MemoryState* memory;
  bool pageBoundaryCrossed(int address, int offset);
  void setNZ(int value);

  // Addressing modes
  
  int addrZp(int arg1, int arg2);
  int addrZpx(int arg1, int arg2);
  int addrZpy(int arg1, int arg2);
  int addrAbs(int arg1, int arg2);
  int addrAbsx(int arg1, int arg2);
  int addrAbsy(int arg1, int arg2);
  int addrIndx(int arg1, int arg2);
  int addrIndy(int arg1, int arg2);
 public:
  CpuRegisters(void);
  ~CpuRegisters(void);
  bool RunInstruction();
  int getA();
  int getX();
  int getY();
  int getS();
  int getPC();
  bool getN();
  bool getZ();
  bool getC();
  bool getD();
  bool getI();
  bool getV();
  void setA(int value); // These setters are for unit tests only
  void setX(int value);
  void setY(int value);
  void setS(int value);
  void setPC(int value);
  MemoryState* getMemory();
};

