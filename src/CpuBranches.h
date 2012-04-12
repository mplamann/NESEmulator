#pragma once
#include "CpuArithmetic.h"
#include "Util.h"

class CpuBranches : public CpuArithmetic
{
 private:
  typedef CpuArithmetic super;
  void doBranch(bool condition, int offset);
  void doJump(int target);
 public:
  CpuBranches(void);
  ~CpuBranches(void);
  bool RunInstruction();
};
