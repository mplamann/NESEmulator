#include "MemoryState.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

#include "Mapper0.h"
#include "Mapper2.h"
using namespace std;

MemoryState::MemoryState(void)
{
  isPpuScrollOnX = true;
  PPUSCROLLX = 0;
  PPUSCROLLY = 0;
  for (int i = 0; i < RAM_SIZE; i++)
    RAM[i] = 0;
  for (int i = 0; i < 0x20; i++)
    palette[i] = 0;
  for (int i = 0; i < 0x400; i++)
    nametable1[i] = nametable2[i] = 0;
}

MemoryState::~MemoryState(void)
{
}

void MemoryState::setGamepad(GamepadState* gpad)
{
  gamepad = gpad;
}

////////////////////////////////////////////////////////////////////////
// Read/Write Operations
////////////////////////////////////////////////////////////////////////

int MemoryState::readByteFrom(int address)
{
  if (address < 0x2000)
    {
      return (RAM[address % 0x800]) & 0xFF;
    }
  else if (address < 0x5000)
    {
      switch (address)
	{
	case 0x2002:
	  {
	    PPUADDR = -1;
	    return PPUSTATUS & 0xFF;
	  }
	case 0x2004:
	  return OAM[OAMADDR] & 0xFF;
	case 0x2007:
	  return ppuReadByteFrom(PPUADDR) & 0xFF;
	case 0x4016:
	  return gamepad->readPlayer1() & 0xFF;
	case 0x4017:
	  return gamepad->readPlayer2() & 0xFF;
	default:
	  return -1;
	}
    }
  else
    {
      return (mapper->readByteFrom(address)) & 0xFF;
    }
}

void MemoryState::writeByteTo(int address, int value)
{
  //  cout << "CPU Write $" << address << " = " << value << "\n";
  if (address < 0x2000)
    RAM[address] = (value & 0xFF); // Each byte only holds 8 bits of data
  else if (address < 0x5000)
    {
      switch (address)
	{
	case 0x2000:
	  PPUCTRL = (value & 0xFF);
	  /*if (PPUCTRL & 0x04)
	    cout << "WARNING: VW Bit set. PPU probably won't work right.\n";*/
	  break;
	case 0x2001:
	  PPUMASK = (value & 0xFF);
	  break;
	case 0x2002:
	  PPUSTATUS = (value & 0xFF);
	  break;
	case 0x2003:
	  OAMADDR = (value & 0xFF);
	  break;
	case 0x2004:
	  OAM[(OAMADDR%256)] = (value & 0xFF);
	  OAMADDR++;
	  break;
	case 0x2005:
	  if (isPpuScrollOnX)
	    PPUSCROLLX = (value & 0xFF);
	  else
	    PPUSCROLLY = (value & 0xFF);
	  isPpuScrollOnX = !isPpuScrollOnX;
	  break;
	case 0x2006:
	  if (PPUADDR == -1)
	    PPUADDR = (value & 0xFF) << 8;
	  else
	    PPUADDR += (value & 0xFF);
	  break;
	case 0x2007:
	  ppuWriteByteTo(PPUADDR,value);
	  if (!(PPUCTRL & 0x04))
	    PPUADDR++;
	  else
	    PPUADDR += 32;
	  break;
	case 0x4014:                   // DMA
	  DMA(value);
	case 0x4016:
	  if (JOYSTROBE == 1 && value == 0)
	    gamepad->strobe();
	  JOYSTROBE=value;
	  break;
	case 0x6004:
	  cout << value;
	default:
	  return; // Unimplemented behavior
	}
    }
  else
    {
      mapper->writeByteTo(address, value);
    }
}

int MemoryState::ppuReadByteFrom(int address)
{
  if (address < 0x2000)
    return (mapper->ppuReadByteFrom(address)) & 0xFF;
  else if (address < 0x2400)
    return readFromNametable(0,address) & 0xFF;
  else if (address < 0x2800)
    return readFromNametable(1,address) & 0xFF;
  else if (address < 0x2C00)
    return readFromNametable(2,address) & 0xFF;
  else if (address < 0x3000)
    return readFromNametable(3,address) & 0xFF;
  else if (address < 0x3F00)
    return ppuReadByteFrom(address - 0x1000) & 0xFF;
  else
    return palette[(address-0x3F20) % 0x20] & 0xFF; // Rest of RAM is just palette mirrored
}

void MemoryState::ppuWriteByteTo(int address, int value)
{
  if (address < 0x2000)
    mapper->ppuWriteByteTo(address, value & 0xFF);
  else if (address < 0x2400)
    writeToNametable(0,address,value & 0xFF);
  else if (address < 0x2800)
    writeToNametable(1,address,value & 0xFF);
  else if (address < 0x2C00)
    writeToNametable(2,address,value & 0xFF);
  else if (address < 0x3000)
    writeToNametable(3,address,value & 0xFF);
  else if (address < 0x3F00)
    ppuWriteByteTo(address - 0x1000, value & 0xFF);
  else
      palette[(address-0x3F00) % 0x20] = value & 0xFF; // Rest of RAM is just palette mirrored
}

int MemoryState::readFromNametable(int nametable, int address)
{
  //if (address > 0x23C8)
  //return 0;
  int currentNametable = 1;
  if (mirroring == 0) // Horizontal Mirroring
    {
      if (nametable > 1)
	currentNametable = 2;
    }
  else if (mirroring == 1) // Vertical Mirroring
    {
      if (nametable == 1 || nametable == 3)
	currentNametable = 2;
    }
  else
    {
    } // TODO: Single-Screen Mirroring - needs mapper support
  int arrayAddress = (address - 0x2000) % 0x400;
  if (currentNametable == 1)
    return nametable1[arrayAddress] & 0xFF;
  else
    return nametable2[arrayAddress] & 0xFF;
}

void MemoryState::DMA(int address)
{
  for (int i = 0; i < 0xFF; i++)
    {
      int currentAddress = (address << 8) + i;
      writeByteTo(0x2004,readByteFrom(currentAddress));
    }
  return;
}

void MemoryState::writeToNametable(int nametable, int address, int value)
{
  //  cout << "Nametable written to at address " << hex << uppercase << address << "\n";
  int currentNametable = 1;
  if (mirroring == 0) // Horizontal Mirroring
    {
      if (nametable > 1)
	currentNametable = 2;
    }
  else if (mirroring == 1) // Vertical Mirroring
    {
      if (nametable == 1 || nametable == 3)
	currentNametable = 2;
    }
  else
    {
    } // TODO: Single-Screen Mirroring - needs mapper support
  int arrayAddress = (address - 0x2400) % 0x400;
  if (currentNametable == 1)
    nametable1[arrayAddress] = value;
  else
    nametable2[arrayAddress] = value;
}

int MemoryState::oamReadByteFrom(int address)
{
  return OAM[address] & 0xFF;
}

void MemoryState::oamWriteByteTo(int address, int value)
{
  OAM[address] = (value & 0xFF);
}

char MemoryState::colorForPaletteIndex(bool isSprite, int paletteIndex, int index)
{
  int baseAddress = 0;
  if (isSprite)
    baseAddress += 0x10;
  baseAddress += paletteIndex*4;
  return palette[baseAddress+index];
}

////////////////////////////////////////////////////////////////////////
// File IO
////////////////////////////////////////////////////////////////////////

void MemoryState::loadFileToRAM(char* filename)
{
  FILE* fileStream;
  size_t size;
  size_t result;
  char* file;

  cout << "Loading ROM ";

  fileStream = fopen(filename, "rb");
  if (fileStream == NULL)
    {
      cout << "Could not read file. ROM not loaded.";
      return;
    }

  // obtain file size
  fseek(fileStream, 0, SEEK_END);
  size = ftell(fileStream);
  rewind(fileStream);

  file = (char*)malloc(sizeof(char)*size);
  if (file == NULL)
    {
      cout << "Memory error. ROM not loaded.";
      fclose(fileStream);
      return;
    }

  result = fread (file,1,size,fileStream);
  if (result != size)
    {
      cout << "Reading error. ROM not loaded.";
      fclose(fileStream);
      return;
    }
  fclose(fileStream);

  bool isROM = (file[0] == 'N' && file[1] == 'E' && file[2] == 'S' && file[3] == 0x1A);
  if (!isROM)
    {
      cout << "This is not a NES rom.";
      return;
    }

  int mapperNumber = ((file[6] & 0xF0) >> 4) + (file[7] & 0xF0);
  cout << "With mapper #" << mapperNumber << " ";

  switch (mapperNumber)
    {
    case 0:
      mapper = new Mapper0(file);
      break;
    case 2:
      mapper = new Mapper2(file);
      break;
    default:
      mapper = new Mapper(file);
      break;
    }

  mirroring = file[6] & 0x9;

  cout << "Done.\n";
  
  return;
}
