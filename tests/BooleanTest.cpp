#include "CppUnitLite/TestHarness.h"
#include "CpuBoolean.h"
#include "Util.h"

TEST (BooleanTest,AND_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Imm);
  mem->writeByteTo(1,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Zp);
  mem->writeByteTo(1,0x12);
  mem->writeByteTo(0x12,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Zpx);
  mem->writeByteTo(1,0x11);
  mem->writeByteTo(0x12,0xA);
  cpu->setA(0xE);
  cpu->setX(0x01);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Abs);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x010A, 0xF);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xE);
}

TEST (BooleanTest,AND_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Absx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x011A,0xA);
  cpu->setA(0xE);
  cpu->setX(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Absy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x01);
  mem->writeByteTo(0x011A,0xA);
  cpu->setA(0xE);
  cpu->setY(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Indx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xB,0xA);
  mem->writeByteTo(0xC,0x01);
  mem->writeByteTo(0x010A,0xA);
  cpu->setA(0xE);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,AND_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,AND_Indy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x01);
  mem->writeByteTo(0xB,0x02);
  mem->writeByteTo(0x0211,0xA);
  cpu->setA(0xE);
  cpu->setY(0x10);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xA);
}

TEST (BooleanTest,EOR_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Imm);
  mem->writeByteTo(1,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Zp);
  mem->writeByteTo(1,0xAA);
  mem->writeByteTo(0xAA,0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Zpx);
  mem->writeByteTo(1,0xA9);
  mem->writeByteTo(0xAA,0xA);
  cpu->setA(0xE);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Abs);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x41);
  mem->writeByteTo(0x410A, 0xA);
  cpu->setA(0xE);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Absx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x12);
  cpu->setA(0xE);
  cpu->setX(0xFF);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Absy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(2,0x12);
  cpu->setA(0xE);
  cpu->setY(0xFF);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Indx);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x12);
  mem->writeByteTo(0xB,0x11);
  mem->writeByteTo(0xC,0x10);
  mem->writeByteTo(0x1011,0xA);
  cpu->setA(0xE);
  cpu->setX(0x1);
  mem->writeByteTo(0x1309,0xA);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

TEST (BooleanTest,EOR_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,EOR_Indy);
  mem->writeByteTo(1,0xA);
  mem->writeByteTo(0xA,0x12);
  mem->writeByteTo(0xB,0x17);
  mem->writeByteTo(0x1713,0xA);
  cpu->setA(0xE);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x4);
}

// ORA Instruction

TEST (BooleanTest, ORA_Imm)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Imm);
  mem->writeByteTo(1,0xD);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xF);
}

TEST (BooleanTest, ORA_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Zp);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xD,0x9);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Zpx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xE,0x9);
  cpu->setA(3);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Abs);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Absx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420F,0x9);
  cpu->setA(3);
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Absy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Absy);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(2,0x42);
  mem->writeByteTo(0x420E,0x9);
  cpu->setA(3);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Indx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Indx);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xE,0xD);
  mem->writeByteTo(0xF,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

TEST (BooleanTest, ORA_Indy)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ORA_Indy);
  mem->writeByteTo(1,0xD);
  mem->writeByteTo(0xD,0x0C);
  mem->writeByteTo(0xE,0x42);
  mem->writeByteTo(0x420D,0x9);
  cpu->setA(3);
  cpu->setY(1);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xB);
}

// ASL Instruction

TEST (BooleanTest, ASL_A)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ASL_A);
  cpu->setA(0xF0);
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xE0);
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ASL_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ASL_Zp);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(0x64,0xFB); // 1111 1011
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0xF6);  // 1111 0110
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ASL_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ASL_Zpx);
  mem->writeByteTo(1,0x62);
  mem->writeByteTo(0x64,0x3B); // 0011 1011
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0x76);  // 0111 0110
  CHECK(cpu->getC() == false);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ASL_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ASL_Abs);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x11);
  mem->writeByteTo(0x1164,0x80); // 1000 0000
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1164) == 0x00);  // 0000 0000
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == true);
}

TEST (BooleanTest, ASL_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ASL_Absx);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x12);
  mem->writeByteTo(0x1265,0xFB); // 1111 1011
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1265) == 0xF6);  // 1111 0110
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

// LSR Instruction

TEST (BooleanTest, LSR_A)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LSR_A);
  cpu->setA(0xF0); // 1111 0000
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x78); // 0111 1000
  CHECK(cpu->getC() == false);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, LSR_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LSR_Zp);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(0x64,0xFB); // 1111 1011
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0x7D);  // 0111 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, LSR_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LSR_Zpx);
  mem->writeByteTo(1,0x62);
  mem->writeByteTo(0x64,0x3B); // 0011 1011
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0x1D);  // 0001 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, LSR_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LSR_Abs);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x11);
  mem->writeByteTo(0x1164,0x01); // 0000 0001
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1164) == 0x00);  // 0000 0000
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == true);
}

TEST (BooleanTest, LSR_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,LSR_Absx);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x12);
  mem->writeByteTo(0x1265,0xFB); // 1111 1011
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1265) == 0x7D);  // 0111 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

// ROL Instruction

TEST (BooleanTest, ROL_A)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROL_A);
  cpu->setA(0xF0); // 1111 0000
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0xE1); // 1110 0001
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROL_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROL_Zp);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(0x64,0xFB); // 1111 1011
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0xF7);  // 1111 0111
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROL_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROL_Zpx);
  mem->writeByteTo(1,0x62);
  mem->writeByteTo(0x64,0x3B); // 0011 1011
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0x76);  // 0111 0110
  CHECK(cpu->getC() == false);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROL_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROL_Abs);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x11);
  mem->writeByteTo(0x1164,0x00); // 0000 0000
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1164) == 0x00);  // 0000 0000
  CHECK(cpu->getC() == false);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == true);
}

TEST (BooleanTest, ROL_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROL_Absx);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x12);
  mem->writeByteTo(0x1265,0xFB); // 1111 1011
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1265) == 0xF7);  // 1111 0111
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

// ROR Instruction

TEST (BooleanTest, ROR_A)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROR_A);
  cpu->setA(0xF0); // 1111 0000
  cpu->RunInstruction();
  CHECK(cpu->getA() == 0x78); // 0111 1000
  CHECK(cpu->getC() == false);
  CHECK(cpu->getN() == false);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROR_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROR_Zp);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(0x64,0xFB); // 1111 1011
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0xFD);  // 1111 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROR_Zpx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROR_Zpx);
  mem->writeByteTo(1,0x62);
  mem->writeByteTo(0x64,0x3B); // 0011 1011
  cpu->setX(2);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x64) == 0x9D);  // 1001 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROR_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROR_Abs);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x11);
  mem->writeByteTo(0x1164,0x01); // 0000 0001
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1164) == 0x80);  // 1000 0000
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

TEST (BooleanTest, ROR_Absx)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,ROR_Absx);
  mem->writeByteTo(1,0x64);
  mem->writeByteTo(2,0x12);
  mem->writeByteTo(0x1265,0xFB); // 1111 1011
  cpu->setX(1);
  cpu->RunInstruction();
  CHECK(mem->readByteFrom(0x1265) == 0xFD);  // 1111 1101
  CHECK(cpu->getC() == true);
  CHECK(cpu->getN() == true);
  CHECK(cpu->getZ() == false);
}

// BIT Insturction

TEST (CpuBoolean,BIT_Zp)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,BIT_Zp);
  mem->writeByteTo(1,0x17);
  mem->writeByteTo(0x17,0x8F);
  cpu->setA(0x70);
  cpu->RunInstruction();
  CHECK(cpu->getN());
  CHECK(!cpu->getV());
  CHECK(cpu->getZ());
}

TEST (CpuBoolean,BIT_Abs)
{
  CpuBoolean* cpu = new CpuBoolean();
  MemoryState* mem = cpu->getMemory();
  mem->writeByteTo(0,BIT_Abs);
  mem->writeByteTo(1,0x17);
  mem->writeByteTo(2,0x23);
  mem->writeByteTo(0x2317,0x7F);
  cpu->setA(0xFF);
  cpu->RunInstruction();
  CHECK(!cpu->getN());
  CHECK(cpu->getV());
  CHECK(!cpu->getZ());
}
