//#include "StdAfx.h"
#include "CpuState.h"


CpuState::CpuState(void)
{
	memory = new MemoryState();
}


CpuState::~CpuState(void)
{
	delete memory;
}

bool CpuState::RunInstruction()
{
	if (super::RunInstruction())
		return true;
	return false;
	/*
	int opcode = memory->readByteFrom(PC);
	int arg1 = memory->readByteFrom(PC+1);
	//int arg2 = memory->readByteFrom(PC+2);
	switch (opcode)
	{
	case ADC_Imm:
		PC += 2;
		C = ((A + arg1) > 255 || (A + arg1) < -255);
		A += arg1;
		cycles += 2;
		N = (A < 0);
		Z = (A == 0);
		
		break;
	case LDA_Imm:
		PC += 2;
		A = arg1;
		cycles += 2;
		//updateFlags(A);
		break;
	}
	opcode + arg1;*/
}

/*void CpuState::updateFlags(byte register) // TODO: specify which flags are modified
{
	if (A < 0)
		N = true;
	else
		N = false;
	if (A == 0)
		Z = true;
	else
		Z = false;
	// TODO: Figure out carry
	// TODO: FIgure out overflow
}*/
