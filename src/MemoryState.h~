#pragma once

#include "Util.h"
#define RAM_SIZE 2048

class MemoryState
{
private:
	byte RAM[RAM_SIZE]; // This machine currently has 2kB of RAM
	                  // I must update this later to reflect the true layout of the NES (http://fms.komkon.org/EMUL8/NES.html)
public:
	MemoryState(void);
	~MemoryState(void);

	byte readByteFrom(unsigned __int16 address);
	void writeByteTo(unsigned __int16 address, byte value);

	void loadFileToRAM(char* filename);
};

