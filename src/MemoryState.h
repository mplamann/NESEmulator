#pragma once

#include "Util.h"
#include "Mapper.h"
#include "GamepadState.h"
#include "ApuState.h"

#define RAM_SIZE 0x2000

class CpuState;

class MemoryState
{
private:
  Mapper* mapper;
  GamepadState* gamepad;
  ApuState* apu;
  CpuState* cpu;
  unsigned char RAM[RAM_SIZE];
  unsigned char palette[0x20];
  unsigned char nametable1[0x400];
  unsigned char nametable2[0x400];
  
  int ppuDataBuffer;
  
  unsigned char OAM[256];
  unsigned char JOYSTROBE;
  bool isPpuScrollOnX;
  bool isPpuAddrHigh;

  void DMA(int address);
public:
  MemoryState(void);
  ~MemoryState(void);
  void setGamepad(GamepadState* gpad);
  void setApu(ApuState* apu);
  void setCpu(CpuState* cpu);
  
  int readByteFrom(int address);
  void writeByteTo(int address, int value);
  
  void loadFileToRAM(char* filename);

  int ppuReadByteFrom(int address);
  void ppuWriteByteTo(int address, int value);

  int apuReadByteFrom(void* user_data, unsigned address);
  
  int oamReadByteFrom(int address);
  void oamWriteByteTo(int address, int value);

  unsigned char* mirroredNametableAtXY(int x, int y);
  
  int readFromNametable(int nametable, int address);
  void writeToNametable(int nametable, int address, int value);

  int getNametableEntryForTile(int x, int y, int xScroll, int yScroll);
  int attributeEntryForXY(int x, int y, int xScroll, int yScroll);

  unsigned char colorForPaletteIndex(bool isSprite, int palette, int index);

  // PPU Registers
  unsigned char PPUCTRL;
  unsigned char PPUMASK;
  unsigned char PPUSTATUS;
  unsigned char OAMADDR;
  unsigned char PPUSCROLLX;
  unsigned char PPUSCROLLY;
  unsigned char PPU_LAST_WRITE;
  int PPUADDR;
};

