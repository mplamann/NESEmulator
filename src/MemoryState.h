#pragma once

#include "Mapper.h"
#include "GamepadState.h"
#include "ApuState.h"
#include <fstream>
using namespace std;

#define RAM_SIZE 0x2000

class CpuV2;

class MemoryState
{
private:
  Mapper* mapper;
  GamepadState* gamepad;
  ApuState* apu;
  CpuV2* cpu;
  unsigned char RAM[RAM_SIZE];
  unsigned char palette[0x20];
  unsigned char nametable1[0x400];
  unsigned char nametable2[0x400];
  
  int ppuDataBuffer;
  
  unsigned char OAM[256];
  unsigned char JOYSTROBE;
  bool ppuLatch;

  void DMA(int address);
public:
  MemoryState(void);
  ~MemoryState(void);
  void setGamepad(GamepadState* gpad);
  void setApu(ApuState* apu);
  void setCpu(CpuV2* cpu);

  void saveState(ofstream file);
  char* stateData(size_t* size);
  void loadState(char* state, size_t size);
  void saveBattery(char* filename);
  void loadBattery(char* filename);
  
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

  void scanlineCounter();

  // PPU Registers
  unsigned char PPUCTRL;
  unsigned char PPUMASK;
  unsigned char PPUSTATUS;
  unsigned char OAMADDR;
  unsigned char PPU_LAST_WRITE;
  int PPUADDR;

  int loopyT;
  int loopyX;
};

