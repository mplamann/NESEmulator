#include "StdAfx.h"
#include "CpuArithmetic.h"


CpuArithmetic::CpuArithmetic(void)
{
}


CpuArithmetic::~CpuArithmetic(void)
{
}

bool CpuArithmetic::pageBoundaryCrossed(int address, int offset)
{
	int newAddress = address + offset;
	if ((newAddress & 0xF0) != (address & 0xF0))
		return true;
	return false;
}

bool CpuArithmetic::RunInstruction()
{
	if (super::RunInstruction())
		return true;
	byte opcode = memory->readByteFrom(PC);
	byte arg1 = memory->readByteFrom(PC+1);
	byte arg2 = memory->readByteFrom(PC+2);
	switch (opcode)
	{
	case ADC_Imm:
		PC += 2;
		cycles += 2;
		addToA(arg1);
		break;
	case ADC_Zp:
		PC += 2;
		cycles += 3;
		addToA(memory->readByteFrom(arg1));
		break;
	case ADC_Zpx:
		PC += 2;
		cycles += 4;
		addToA(memory->readByteFrom(arg1+X));
		break;
	case ADC_Abs:
		addToAAbs(arg1, arg2, 0);
		break;
	case ADC_Absx:
		addToAAbs(arg1, arg2, X);
		break;
	case ADC_Absy:
		addToAAbs(arg1, arg2, Y);
		break;
	}
}

void CpuArithmetic::addToAAbs(byte arg1, byte arg2, byte offset)
{
	PC += 3;
	cycles += 4;
	if (pageBoundaryCrossed(arg1+arg2<<2,offset))
		cycles += 1;
	addToA(memory->readByteFrom(arg1+arg2<<2+offset));
}

void CpuArithmetic::addToA(byte value)
{
	unsigned int temp = value + A + (C ? 1 : 0);
	C = (temp > 0xFF);
	N = (temp < 0);
	Z = (temp == 0);
	V = (!((A ^ value) & 0x80) && ((A ^ temp) & 0x80));
	A = (byte)temp;
}