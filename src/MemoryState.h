#pragma once

#include "Util.h"
#include "Mapper.h"
#include "GamepadState.h"

#define RAM_SIZE 0x2000

class MemoryState
{
private:
  Mapper* mapper;
  GamepadState* gamepad;
  char RAM[RAM_SIZE];
  char palette[0x20];
  char nametable1[0x400];
  char nametable2[0x400];
  int mirroring;
  char OAM[256];
  char JOYSTROBE;
  bool isPpuScrollOnX;

  void DMA(int address);
public:
  MemoryState(void);
  ~MemoryState(void);
  void setGamepad(GamepadState* gpad);
  
  int readByteFrom(int address);
  void writeByteTo(int address, int value);
  
  void loadFileToRAM(char* filename);

  int ppuReadByteFrom(int address);
  void ppuWriteByteTo(int address, int value);

  int oamReadByteFrom(int address);
  void oamWriteByteTo(int address, int value);

  char* mirroredNametableAtXY(int x, int y);
  
  int readFromNametable(int nametable, int address);
  void writeToNametable(int nametable, int address, int value);

  char colorForPaletteIndex(bool isSprite, int palette, int index);

  // PPU Registers
  char PPUCTRL;
  char PPUMASK;
  char PPUSTATUS;
  unsigned char OAMADDR;
  char PPUSCROLLX;
  char PPUSCROLLY;
  int PPUADDR;
};

