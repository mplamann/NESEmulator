#pragma once

#include "Util.h"
#define RAM_SIZE 64*1024

class MemoryState
{
private:
	char RAM[RAM_SIZE]; // This machine currently has 2kB of RAM
	                  // I must update this later to reflect the true layout of the NES (http://fms.komkon.org/EMUL8/NES.html)
public:
	MemoryState(void);
	~MemoryState(void);

	int readByteFrom(int address);
	void writeByteTo(int address, int value);

	void loadFileToRAM(char* filename);
};

