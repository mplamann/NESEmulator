#include "StdAfx.h"
#include "CpuRegisters.h"


CpuRegisters::CpuRegisters(void)
{
	A=0;
	X=0;
	Y=0;
	S=0;
	PC=0;
	cycles = 0;

	C = false; // Carry flag
	Z = false; // Zero flag
	I = false; // Interrupt flag
	D = false; // Decimal flag, not used
	B = false; // BRK flag
	V = false; // Overflow flag
	S = false; // Sign flag (1 if negative, 0 if positive)
}


CpuRegisters::~CpuRegisters(void)
{
}

bool CpuRegisters::RunInstruction()
{

}