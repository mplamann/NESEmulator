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
 public:
  int readByteFrom(int address);
  virtual void writeByteTo(int address, int value);
  int ppuReadByteFrom(int address);
  virtual void ppuWriteByteTo(int address, int value);
  void updatePpuAddr(int address) {}
  int mirroring;

  Mapper(char* file);
  ~Mapper(void);
};
