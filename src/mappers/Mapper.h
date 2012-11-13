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
  void writeByteTo(int address, int value);
  int ppuReadByteFrom(int address);
  void ppuWriteByteTo(int address, int value);
  int mirroring;

  Mapper(char* file);
  ~Mapper(void);
};
