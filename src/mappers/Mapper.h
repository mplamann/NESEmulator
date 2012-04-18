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
  int readPpuByteFrom(int address);
  void writePpuByteTo(int address);

  Mapper(char* file);
  ~Mapper(void);
};
