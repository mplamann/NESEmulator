// NESEmulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CpuState.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CpuState* cpu = new CpuState();
	cpu->RunInstruction();
	cpu->RunInstruction();
	return 0;
}

