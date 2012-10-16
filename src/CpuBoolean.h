#pragma once
#include "CpuRegisters.h"
#include "Util.h"

class CpuBoolean : public CpuRegisters
{
 private:
  typedef CpuRegisters super;
protected:
  int ASL(int value);
  int LSR(int value);
  int ROL(int value);
  int ROR(int value);
  void BIT(int value);
  void SLO(int value);
  void RLA(int value);
  void SRE(int value);
 public:
  CpuBoolean(void);
  ~CpuBoolean(void);
  bool RunInstruction();
};
