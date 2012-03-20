#pragma once
#include "CpuRegisters.h"
#include "Util.h"
class CpuArithmetic : public CpuRegisters
{
private:
	typedef CpuRegisters super;
	void addToA(byte value);
	void addToAAbs(byte arg1, byte arg2, byte offset);
protected:
	bool pageBoundaryCrossed(int address, int offset);
public:
	CpuArithmetic(void);
	~CpuArithmetic(void);
	bool RunInstruction();
};

