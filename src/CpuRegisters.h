#pragma once
#include "Util.h"
#include "MemoryState.h"
class CpuRegisters
{
private:
        void setNZ(int value);
protected:
	int A;
	int X;
	int Y;
	int S;
	int PC;

	bool N,Z,C,I,D,V,B; // Processor status flags

	int cycles;
	MemoryState* memory;

public:
	CpuRegisters(void);
	~CpuRegisters(void);
	bool RunInstruction();
};

