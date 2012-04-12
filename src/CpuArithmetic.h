#pragma once
#include "CpuBoolean.h"
#include "Util.h"
class CpuArithmetic : public CpuBoolean
{
 private:
  typedef CpuBoolean super;
  void addToA(int value);
  void subFromA(int value);
  void cmpMReg(int memory, int reg);
 public:
  CpuArithmetic(void);
  ~CpuArithmetic(void);
  bool RunInstruction();
};

