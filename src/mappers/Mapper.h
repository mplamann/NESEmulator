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

  int prgBank1Index;
  int prgBank2Index;
  int chrBank1Index;
  int chrBank2Index;
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

  Mapper(char* file);
  ~Mapper(void);
};
