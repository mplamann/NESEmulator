#pragma once

class Mapper
{
  
 public:
  virtual int readByteFrom(int address) = 0;
  virtual int writeByteTo(int address) = 0;
};
