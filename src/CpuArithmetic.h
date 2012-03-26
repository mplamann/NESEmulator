#pragma once
#include "CpuBoolean.h"
#include "Util.h"
class CpuArithmetic : public CpuBoolean
{
 private:
  typedef CpuBoolean super;
  void addToA(int value);
  void subFromA(int value);
 public:
  CpuArithmetic(void);
  ~CpuArithmetic(void);
  bool RunInstruction();
};

