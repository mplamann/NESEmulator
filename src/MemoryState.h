#pragma once

#include "Util.h"
#define RAM_SIZE 0x800

class MemoryState
{
private:
	char RAM[RAM_SIZE];
public:
	MemoryState(void);
	~MemoryState(void);

	int readByteFrom(int address);
	void writeByteTo(int address, int value);

	void loadFileToRAM(char* filename);
};

