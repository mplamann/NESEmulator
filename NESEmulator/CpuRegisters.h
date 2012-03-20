#pragma once
#include "Util.h"
#include "MemoryState.h"
class CpuRegisters
{
protected:
	byte A;
	byte X;
	byte Y;
	byte S;
	byte PC;

	bool N,Z,C,I,D,V,B; // Processor status flags

	int cycles;
	MemoryState* memory;

public:
	CpuRegisters(void);
	~CpuRegisters(void);
	bool RunInstruction();
};

