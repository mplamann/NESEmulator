#pragma once
#include "CpuRegisters.h"
#include "Util.h"
class CpuArithmetic : public CpuRegisters
{
private:
	typedef CpuRegisters super;
	void addToA(int value);
	void addToAAbs(int arg1, int arg2, int offset);
protected:
	bool pageBoundaryCrossed(int address, int offset);
public:
	CpuArithmetic(void);
	~CpuArithmetic(void);
	bool RunInstruction();
};

