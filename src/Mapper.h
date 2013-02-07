#include <cstddef>

#pragma once

class Mapper
{
 protected:
  char** prgBanks;
  char** chrBanks;
  int nPrgBanks;
  int nChrBanks;
  int mapperNumber;
  int prgBankSize;
  int chrBankSize;
  int* prgIndexes;
  int* chrIndexes;

  bool batteryBacked;
  int prgRam[0x2000];
  bool prgRamEnabled;
  bool prgRamWritable;

 public:
  virtual int readByteFrom(int address);
  virtual void writeByteTo(int address, int value);
  virtual int ppuReadByteFrom(int address);
  virtual void ppuWriteByteTo(int address, int value);
  virtual void updatePpuAddr(int) {}
  int mirroring;

  virtual size_t stateSize();
  virtual char* stateData();
  virtual void loadState(char* state);

  virtual void saveBattery(char* filename) {}
  virtual void loadBattery(char* filename) {}

  virtual void scanlineCounter();

  Mapper(char* file, int prgBankSize = 16, int chrBankSize = 4);
  ~Mapper(void);
};
