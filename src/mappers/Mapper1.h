#pragma once

#include "Mapper.h"
#include "Util.h"

const int PRG_SWITCH_32 = 0;
const int PRG_SWITCH_FIRST_16 = 3;
const int PRG_SWITCH_LAST_16 = 2;

const int CHR_SWITCH_8 = 0;
const int CHR_SWITCH_2x4 = 1;

class Mapper1 : public Mapper
{
 private:
  int prgRam[0x2000];
  bool prgRamEnabled;
  int prgBankIndex;
  int shiftRegister;
  int shiftIndex;

  int prgBankMode;
  int chrBankMode;

  void writeControl(int value);
  void writeCHR0(int value);
  void writeCHR1(int value);
  void writePRG(int value);
  void updatePRGIndexes();
  
 public:
  Mapper1(char* file);
  ~Mapper1(void);
  void writeByteTo(int address, int value);
  int readByteFrom(int address);

  size_t stateSize();
  char* stateData();
  void loadState(char* buffer);

  void saveBattery(char* filename);
  void loadBattery(char* filename);
};
