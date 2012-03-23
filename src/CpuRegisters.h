#pragma once
#include "Util.h"
#include "MemoryState.h"
class CpuRegisters
{
private:
        void setNZ(int value);
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
public:
	CpuRegisters(void);
	~CpuRegisters(void);
	bool RunInstruction();
};

