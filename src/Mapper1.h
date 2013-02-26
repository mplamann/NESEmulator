#pragma once

#include "Mapper.h"
#include "Util.h"
#include <fstream>
using namespace std;

const int PRG_SWITCH_32 = 0;
const int PRG_SWITCH_FIRST_16 = 3;
const int PRG_SWITCH_LAST_16 = 2;

const int CHR_SWITCH_8 = 0;
const int CHR_SWITCH_2x4 = 1;

class Mapper1 : public Mapper
{
 private:
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

  void saveState(ofstream& file);
  void loadState(ifstream& file);

  void saveBattery(char* filename);
  void loadBattery(char* filename);
};
