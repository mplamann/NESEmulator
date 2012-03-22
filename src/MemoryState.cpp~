//#include "StdAfx.h"
#include "MemoryState.h"
#include <iostream>
#include <fstream>
using namespace std;

MemoryState::MemoryState(void)
{
	for (int i = 0; i < 2048; i++)
		RAM[i] = 0;
	RAM[0] = 0xA9;
	RAM[1] = 0x17;
	RAM[2] = 0x69;
	RAM[3] = 0x17;
}

MemoryState::~MemoryState(void)
{
}

int MemoryState::readByteFrom(int address)
{
	return RAM[address];
}

void MemoryState::writeByteTo(int address, int value)
{
	RAM[address] = value;
}

void MemoryState::loadFileToRAM(char* filename)
{
	ifstream file (filename, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		ifstream::pos_type size = file.tellg();
		if (size > RAM_SIZE)
			size = RAM_SIZE;
		cout << "File too large, truncating.";
		file.seekg(0, ios::beg);
		file.read((char*)RAM, size);
		file.close();
	}
}
